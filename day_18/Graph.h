#pragma once

#include <map>
#include <string>

#include "Tile.h"
#include "Image.h"

struct Graph
{
	using AdjacencyList = std::map<Tile, TileSet, TileComparator>;

	std::string draw() const;

	TileMap tileMap;
	AdjacencyList adjacencyList;

	std::map<char, Tile> poiMap;
};


class GraphBuilder
{
public:
	static Graph buildGraph(const Image & img, const Tile & start);
	static Graph buildGraph(const Image & img, const std::vector<Tile> & start);

private:
	GraphBuilder(const Image & img);

	void startExplore(const Tile & start);
	void explore(Direction previous = Still);
	bool advance(Direction d);

private:
	Tile m_currentTile;

	Image m_img;
	Graph m_graph;

	std::map<Tile, std::set<Direction>, TileComparator> m_visited;
};