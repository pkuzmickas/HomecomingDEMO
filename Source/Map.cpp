#include "Map.h"

using namespace rapidxml;
using namespace std;
/*
Method to read a file and store the data into a vector buffer.
Returns true if successful
*/
bool Map::readFile(std::string path, std::vector<char> &buffer) {

	ifstream theFile(path);
	vector<char> buffer1((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer1.push_back('\0');
	buffer = buffer1;
	if (buffer.at(0) == '\0') {
		return false;
	}
	return true;
}
/*
Returns a buffer with the data for the map.
*/
std::vector<char> Map::loadMap(std::string path) {
	std::vector<char> buffer;
	if (!readFile(path, buffer)) { 
		std::cout << "Failed to read the map!" << std::endl;
		std:vector<char> empty;
		return empty;
	}
	return buffer;
}

Map::Map(SDL_Renderer* renderer, std::string path) {
	gRenderer = renderer;
	path = (string)ASSET_DIR + path;
	std::vector<char> map = loadMap(path);
	if (map.empty()) return;

	doc.parse<0>(&map[0]);
	root_node = doc.first_node("map");

	width = stoi((string)root_node->first_attribute("width")->value());
	height = stoi((string)root_node->first_attribute("height")->value());
	levelWidth = Globals::TILE_SIZE * width;
	levelHeight = Globals::TILE_SIZE * height;

	xml_node<>* layer = root_node->first_node("tileset");
	tilecount = stoi((string)layer->first_attribute("tilecount")->value());
	columns = stoi((string)layer->first_attribute("columns")->value());

	layer = root_node->first_node("tileset")->first_node("image");
	spriteSheetPath = (string)layer->first_attribute("source")->value();
	ssWidth = stoi((string)layer->first_attribute("width")->value());
	ssHeight = stoi((string)layer->first_attribute("height")->value());

	string temp = ASSET_DIR;
	spriteSheet = IMG_LoadTexture(gRenderer, (((string)ASSET_DIR) + spriteSheetPath).c_str());

	// loading low collision map AND the map matrix for the tile entities

	xml_node<> * tile_gid = root_node->first_node("layer")->first_node("data")->first_node("tile");
	for (int i = 0; i < height; i++) {
		std::vector<vector<Entity*>> row;
		for (int j = 0; j < width; j++) {
			std::vector<Entity*> col;
			Entity* tile = new Entity();
			col.push_back(tile); // push it onto the Entity list in the location (list is required if there are couple of entities on top of each other representing layers)
			row.push_back(col); // push the list into the map position 
			Transform* tileTransform = new Transform(tile, Globals::TILE_SIZE, Globals::TILE_SIZE, (int)(j * Globals::TILE_SIZE), (int)(i * Globals::TILE_SIZE));
			tile->addComponent(tileTransform);

			int gidValue = stoi((string)tile_gid->first_attribute("gid")->value());
			if (gidValue != 0) {
				Collider* tileCollider = new Collider(tile);
				tileCollider->colType = Collider::ColliderType::LOW;
				tileCollider->colBox.x = (int)tileTransform->globalPosX;
				tileCollider->colBox.y = (int)tileTransform->globalPosY;
				tileCollider->colBox.h = tileTransform->transformRect.h;
				tileCollider->colBox.w = tileTransform->transformRect.w;
				tile->addComponent(tileCollider);
			}
			tile_gid = tile_gid->next_sibling();

		}
		mapMatrix.push_back(row);
	}

	


	// loading normal collision map

	tile_gid = root_node->first_node("layer")->next_sibling()->first_node("data")->first_node("tile");
	
	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			int gidValue = stoi((string)tile_gid->first_attribute("gid")->value());
			if (gidValue != 0) {
				Entity* tile = mapMatrix[i][j][0]; // We just get the first element of the list of entities since there aren't any layers yet
				Transform* tileTransform = (Transform*)tile->findComponent(ComponentType::TRANSFORM);
				Collider* tileCollider = new Collider(tile);
				tileCollider->colType = Collider::ColliderType::NORMAL;
				tileCollider->colBox.x = (int)tileTransform->globalPosX;
				tileCollider->colBox.y = (int)tileTransform->globalPosY;
				tileCollider->colBox.h = tileTransform->transformRect.h;
				tileCollider->colBox.w = tileTransform->transformRect.w;
				tile->addComponent(tileCollider);
			}
			tile_gid = tile_gid->next_sibling();

		}  
	}
	/*for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cout << mapMatrix[i][j][0]->hasComponent(ComponentType::COLLIDER) ? "yes" : "no";
		}
		cout << endl;
	}*/

	// loading tile drawable component (how a tile looks like)

	layer = root_node->first_node("layer")->next_sibling()->next_sibling();
	int layerNr = 0;
	
	while (layer) {
		layerNr = stoi((string)layer->first_attribute("name")->value());
		tile_gid = layer->first_node("data")->first_node("tile");
		for (int i = 0; i < height; i++) {
			int curListEnt = 0;

			for (int j = 0; j < width; j++) {
				int spriteNr = stoi((string)tile_gid->first_attribute("gid")->value());
				if (spriteNr != 0) {
					while (true) { // in case there already is a drawable and we need to create a new tile on top of all others in the specific coordinate
						Entity* tile = mapMatrix[i][j][curListEnt];
						if (!tile->hasComponent(ComponentType::DRAWABLE)) {
							std::ostringstream oss;
							oss << "tile " << i << " " << j << " " << curListEnt;
							std::string tileID = oss.str();
							SDL_Rect* srcRect = new SDL_Rect();
							srcRect->h = Globals::TILE_SIZE;
							srcRect->w = Globals::TILE_SIZE;
							srcRect->x = (spriteNr % width) * Globals::TILE_SIZE;
							srcRect->y = (spriteNr / width) * Globals::TILE_SIZE;
							Drawable* tileDrawable = new Drawable(tile, spriteSheet, tileID, (Globals::Layers)layerNr, srcRect);
							tile->addComponent(tileDrawable);
							curListEnt = 0;
							break;
						}
						else { // if there is already a tile with DRAWABLE in the location, create another entity on top of it to represent layers
							curListEnt++;
							if (curListEnt == mapMatrix[i][j].size()) {
								Entity* tileOnTop = new Entity();
								mapMatrix[i][j].push_back(tileOnTop);
								Transform* tileTransform = new Transform(tileOnTop, Globals::TILE_SIZE, Globals::TILE_SIZE, (int)(j * Globals::TILE_SIZE), (int)(i * Globals::TILE_SIZE));
								tileOnTop->addComponent(tileTransform);
							}
							
						}
					}
				}

				tile_gid = tile_gid->next_sibling();
				
			}
		}
		layer = layer->next_sibling();
	}
}

Map::~Map() {
	for (auto row : mapMatrix) {
		for (auto col : row) {
			for (auto entity : col) {
				delete entity;
			}
		}
	}
}
