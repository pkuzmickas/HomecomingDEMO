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
	if (!readFile(path, buffer)) { // change into path
		std::cout << "Failed to read the map!" << std::endl;
	}
	return buffer;
}

Map::Map(SDL_Renderer* renderer, std::string path) {
	gRenderer = renderer;
	std::vector<char> map = loadMap(path);
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

	// loading low collision map

	xml_node<> * tile_gid = root_node->first_node("layer")->first_node("data")->first_node("tile");
	//std::vector<Collider*> TODO
	for (int i = 0; i < height; i++) {
		vector<int> row;
		for (int j = 0; j < width; j++) {
			int spriteNr = stoi((string)tile_gid->first_attribute("gid")->value());

			row.push_back(spriteNr);

			tile_gid = tile_gid->next_sibling();

		}
		lowColMap.push_back(row); 
	}

	tile_gid = root_node->first_node("layer")->next_sibling()->first_node("data")->first_node("tile");
	// loading normal collision map
	for (int i = 0; i < height; i++) {
		vector<int> row;
		for (int j = 0; j < width; j++) {
			int spriteNr = stoi((string)tile_gid->first_attribute("gid")->value());

			row.push_back(spriteNr);

			tile_gid = tile_gid->next_sibling();

		}
		colMap.push_back(row);
	}

	layer = root_node->first_node("layer")->next_sibling()->next_sibling();
	int layerNr = 0;

	for (int i = 0; i < Constants::MAX_LAYERS; i++) {
		vector<vector<int>> vec;
		tileMap.push_back(vec);
	}

	while (layer) {
		layerNr = stoi((string)layer->first_attribute("name")->value());
		tile_gid = layer->first_node("data")->first_node("tile");
		for (int i = 0; i < height; i++) {
			vector<int> row;
			for (int j = 0; j < width; j++) {
				int spriteNr = stoi((string)tile_gid->first_attribute("gid")->value());

				row.push_back(spriteNr);

				tile_gid = tile_gid->next_sibling();

			}
			tileMap[layerNr].push_back(row);
			cout << layerNr << endl;
		}
		layer = layer->next_sibling();
	}
}