#pragma once

#include <map>
#include <list>
#include <string>
#include <thread>

#include <Eigen/Core>

#include "Intprogram.h"
#include "InterProgramCommunication.h"

#include "n_comparator.h"

enum TileType
{
	Wall, Free, Oxygen
};

enum Direction
{
	Still = 0,
	North = 1,
	South = 2,
	West = 3,
	East = 4
};

using Tile = Eigen::Vector2i;
using TileComparator = NComparator<Tile, 2>;
using TileMap = std::map<Tile, TileType, TileComparator>;
using TileAdjacencyList = std::map<Tile, std::list<Tile>, TileComparator>;
using TileTypeToCharMapping = std::map<TileType, char>;

struct Graph
{
	std::string draw() const;

	Tile start;
	Tile oxygen;

	TileMap tileMap;
	TileAdjacencyList adjacencyList;

	static const TileTypeToCharMapping charMapping;
};

class Drone
{
public:
	static Graph buildGraph(const std::string & filename);

private:
	Drone();
	~Drone();

	void start(const std::string & filename);
	bool advance(Direction d);
	void explore(Direction previous = Still);
	void stop();

private:
	InterProgramCommunication m_input;
	InterProgramCommunication m_output;
	std::thread m_thread;

	Tile m_currentTile;

	Graph m_graph;
};

using DistanceMap = std::map<Tile, int, TileComparator>;
DistanceMap bfs(const Graph & g, Tile start);
int distStartToOxygen(const Graph & g);
int oxygenDistributionTime(const Graph & g);