#pragma once

#include <map>
#include <string>

#include "Tile.h"
#include "Image.h"



struct DonutStructure
{
	DonutStructure(const Image & img);

	int row_outer_top;
	int row_outer_bottom;

	int col_outer_left;
	int col_outer_right;

	int row_inner_top;
	int row_inner_bottom;

	int col_inner_left;
	int col_inner_right;
};

struct Portals
{
	struct Portal
	{
		Tile innerPortal;
		Tile outerPortal;
	};

	enum LevelChange{In = 1, Out = -1, No = 0};

	Tile start;
	Tile end;

	LevelChange canPortal(const Tile & t) const;
	Tile portal(const Tile & t) const;

	std::map<Tile, Tile, TileComparator> hopOut;
	std::map<Tile, Tile, TileComparator> hopIn;

};

Portals findPortals(const Image & img);

struct Graph
{
	using AdjacencyList = std::map<Tile, TileSet, TileComparator>;

	std::string draw() const;

	TileMap tileMap;
	AdjacencyList adjacencyList;

	Tile start, end;

	std::map<char, Tile> poiMap;
	Portals portals;
};

class GraphBuilder
{
public:
	static Graph buildGraph(const Image & img);
	static Graph buildRecursableGraph(const Image & img);

private:
	GraphBuilder(const Image & img);

	void startExplore();
	void startExploreRecursable();

	void explore(Direction previous = Still);
	void exploreRecursable(Direction previous = Still);

	bool advance(const Tile & t, Direction d);

private:
	Tile m_currentTile;

	Image m_img;
	Graph m_graph;
	Portals m_portals;

	std::map<Tile, std::set<Direction>, TileComparator> m_visited;
};
