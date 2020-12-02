#pragma once

#include <set>

#include "Tile.h"
#include "Graph.h"

struct Distance
{
	int tiles;
	std::set<char> keysNeeded;
};

using DistanceMap = std::map<Tile, Distance, TileComparator>;


class KeyDistanceFunction
{
private:

	using MultiSourceDistanceMap = std::map<Tile, DistanceMap, TileComparator>;

public:
	KeyDistanceFunction(const Graph & g, const Tile & start);
	KeyDistanceFunction(const Graph & g, const std::vector<Tile> & start);

	int distance(const Tile & source, const Tile & target, const std::set<char> keys);
	int distance(char sourceKey, char targetKey, const std::set<char> keys);

	std::map<Tile, int, TileComparator> distances(const Tile & source, const std::set<char> keys);
	std::map<Tile, int, TileComparator> distances(char sourceKey, const std::set<char> keys);

private:
	MultiSourceDistanceMap m_map;
	Graph m_graph;
};

DistanceMap bfs_keybound(const Graph & g, const Tile & start);