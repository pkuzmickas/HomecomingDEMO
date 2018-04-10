#include "MapSystem.h"

using namespace rapidxml;
using namespace std;

int MapSystem::levelHeight = 0;
int MapSystem::levelWidth = 0;

std::vector<std::vector<std::vector<Entity*>>> MapSystem::mapMatrix;
/*
Method to read a file and store the data into a vector buffer.
Returns true if successful
*/
bool MapSystem::readFile(string path, vector<char> &buffer) {

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
vector<char> MapSystem::loadMap(string path) {
	vector<char> buffer;
	if (!readFile(path, buffer)) { 
		cout << "Failed to read the map!" << endl;
		vector<char> empty;
		return empty;
	}
	return buffer;
}

void MapSystem::createMap(SDL_Renderer* renderer, std::string path) {
	path = (string)ASSET_DIR + path;
	std::vector<char> map = loadMap(path);
	if (map.empty()) return;

	rapidxml::xml_node<>* root_node;
	rapidxml::xml_document<> doc;
	doc.parse<0>(&map[0]);
	root_node = doc.first_node("map");

	int width = stoi((string)root_node->first_attribute("width")->value());
	int height = stoi((string)root_node->first_attribute("height")->value());
	levelWidth = Globals::TILE_SIZE * width;
	levelHeight = Globals::TILE_SIZE * height;

	xml_node<>* layer = root_node->first_node("tileset");
	int tilecount = stoi((string)layer->first_attribute("tilecount")->value());
	int columns = stoi((string)layer->first_attribute("columns")->value());

	layer = root_node->first_node("tileset")->first_node("image");
	std::string spriteSheetPath = (string)layer->first_attribute("source")->value();
	int ssWidth = stoi((string)layer->first_attribute("width")->value());
	int ssHeight = stoi((string)layer->first_attribute("height")->value());

	string temp = ASSET_DIR;
	SDL_Texture* spriteSheet = IMG_LoadTexture(renderer, (((string)ASSET_DIR) + spriteSheetPath).c_str());

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
				tile->addComponent(tileCollider);
			}
			tile_gid = tile_gid->next_sibling();

		}  
	}

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
							srcRect->x = ((spriteNr-1) % (int)(ssWidth/Globals::TILE_SIZE)) * Globals::TILE_SIZE; //SpriteNr-1 because spritenr is starts from 1 initialy and we need to start from 0
							srcRect->y = ((spriteNr-1) / (int)(ssWidth/Globals::TILE_SIZE)) * Globals::TILE_SIZE;
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

void MapSystem::deleteMap() {
	for (auto row : mapMatrix) {
		for (auto col : row) {
			for (auto entity : col) {
				delete entity;
			}
		}
	}
}
