#pragma once
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Entity.h"
using namespace std;
class PathfindingSystem {
public:
	struct Node {
		SDL_Point coord;
		int value;
	};
	static vector<SDL_Point> AStar(Node start, Node goal);
	static void initBoard(std::vector<std::vector<std::vector<Entity*>>> mapMatrix);
private:
	PathfindingSystem();
	static bool initialized;
	static vector<vector<Node>> board;
	static vector<SDL_Point> reconstruct_path(unordered_map<Node*, Node*> cameFrom, Node* current);
	static int strCost;
	static int heuristicCostEstimate(Node start, Node finish);
};