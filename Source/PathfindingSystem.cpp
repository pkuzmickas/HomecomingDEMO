#include "PathfindingSystem.h"

bool PathfindingSystem::initialized = false;
int PathfindingSystem::strCost = 10;
vector<vector<PathfindingSystem::Node>> PathfindingSystem::board;

vector<SDL_Point> PathfindingSystem::AStar(Node start, Node goal) {
	if (!initialized) {
		throw "NOT INITIALIZED";
	}
	vector<SDL_Point> result;

	Node* realGoal = &board[goal.coord.y][goal.coord.x];
	if (board[goal.coord.y][goal.coord.x].value != 0 || board[start.coord.y][start.coord.x].value != 0) {
		return result;
	}
	unordered_set<Node*> closedSet;
	unordered_set<Node*> openSet;
	openSet.insert(&start);
	unordered_map<Node*, Node*> cameFrom;
	//to the start
	unordered_map<Node*, int> gScore;
	gScore[&start] = 0;
	//to the finish
	unordered_map<Node*, int> fScore;
	fScore[&start] = heuristicCostEstimate(start, *realGoal);
	while (!openSet.empty()) {
		Node* current = NULL;
		for (auto it : openSet) {
			if (fScore[it]<fScore[current] || !current) {
				current = it;
			}
		}
		if (current == realGoal) {
			return reconstruct_path(cameFrom, current);
		}
		openSet.erase(current);
		closedSet.insert(current);
		for (int i = 1; i < 9; i += 2) {
			if (i == 4) continue;
			int x = current->coord.x - 1 + i % 3;
			int y = current->coord.y - 1 + i / 3;
			if (x < 0 || y < 0 || x >= (int)board[y].size() || y >= (int)board.size()) continue;
			Node* neighbor = &board[y][x];
			if (closedSet.count(neighbor) != 0 || neighbor->value != 0) { // is in set
				continue;
			}
			if (openSet.count(neighbor) == 0) {
				openSet.insert(neighbor);
			}
			int tentativeGScore = gScore[current] + heuristicCostEstimate(*current, *neighbor);
			if (gScore.find(neighbor) == gScore.end()) gScore[neighbor] = 666666;
			if (tentativeGScore >= gScore[neighbor]) continue;

			cameFrom[neighbor] = current;
			gScore[neighbor] = tentativeGScore;
			fScore[neighbor] = gScore[neighbor] + heuristicCostEstimate(*neighbor, goal);
		}

	}

	return result;

}

void PathfindingSystem::initBoard(std::vector<std::vector<std::vector<Entity*>>> mapMatrix) {
	if (initialized) return;
	initialized = true;
	for (int i = 0; i < (int)mapMatrix.size(); i++) {
		vector<Node> v;
		for (int j = 0; j < (int)mapMatrix[i].size(); j++) {
			Node node;
			SDL_Point coord;
			coord.x = j;
			coord.y = i;
			node.coord = coord;
			node.value = 0;
			for (auto ent : mapMatrix[i][j]) {
				if (ent->hasComponent(ComponentType::COLLIDER)) {
					node.value = 1;
				}
			}
			v.push_back(node);
		}
		board.push_back(v);
	}
}

vector<SDL_Point> PathfindingSystem::reconstruct_path(unordered_map<Node*, Node*> cameFrom, Node * current) {
	vector<SDL_Point> totalPath;
	SDL_Point p;
	p.x = current->coord.x;
	p.y = current->coord.y;
	totalPath.push_back(p);

	while (cameFrom[current]) {
		current = cameFrom[current];
		SDL_Point p;
		p.x = current->coord.x;
		p.y = current->coord.y;
		totalPath.push_back(p);
	}
	return totalPath;
}

int PathfindingSystem::heuristicCostEstimate(Node start, Node finish) {
	int cost = 0;
	while (start.coord.x != finish.coord.x || start.coord.y != finish.coord.y) {
		if (finish.coord.y != start.coord.y && finish.coord.x != start.coord.x) {
			cost += strCost * 2;
		}
		else {
			cost += strCost;
		}
		if (finish.coord.x > start.coord.x) {
			start.coord.x++;
		}
		else if (finish.coord.x < start.coord.x) {
			start.coord.x--;
		}
		if (finish.coord.y > start.coord.y) {
			start.coord.y++;
		}
		else if (finish.coord.y < start.coord.y) {
			start.coord.y--;
		}
		if (cost > 1000000) break; // in failure case
	}
	return cost;
}
