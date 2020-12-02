#include "Drone.h"

#include "console_image.h"
#include "read_file.h"

#include <iostream>

namespace
{
	Tile nextTile(Tile t, Direction d)
	{
		switch (d)
		{
			case North:
				return t + Tile{ 0, -1 };
			case South:
				return t + Tile{ 0, 1 };
			case West:
				return t + Tile{ -1, 0 };
			case East:
				return t + Tile{ 1, 0 };
			default:
				throw std::runtime_error("");
		}
	}

	Direction inverse(Direction d)
	{
		switch (d)
		{
			case North:
				return South;
			case South:
				return North;
			case West:
				return East;
			case East:
				return West;
			default:
				throw std::runtime_error("");
		}
	}

} // namespace

const TileTypeToCharMapping Graph::charMapping = { { Oxygen, 'O' }, { Wall, ' ' }, { Free, '.' } };


std::string Graph::draw() const
{
	return generate_image_with_map(tileMap, charMapping, ' ');
}


Drone::Drone() : m_currentTile{ 0, 0 }
{
	m_graph.start = m_currentTile;
	m_graph.adjacencyList[m_currentTile] = std::list<Tile>{};
	m_graph.tileMap[m_currentTile] = Free;
}


Drone::~Drone()
{
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}


void Drone::start(const std::string & filename)
{
	m_thread = std::thread{ [this, &filename] {
		const auto code = read_collection(filename, ",", Converter<Integer>{});
		Intprogram{ m_input, m_output, code }.run();
	} };
}


bool Drone::advance(Direction d)
{
	m_input.write(static_cast<Integer>(d));

	Tile t = nextTile(m_currentTile, d);
	TileType result = static_cast<TileType>(m_output.readNext());

	if (m_graph.tileMap.find(t) == m_graph.tileMap.end())
	{
		m_graph.tileMap[t] = result;
	}

	if (result != Wall)
	{
		if(result == Oxygen)
		{
			m_graph.oxygen = t;
		}

		m_graph.adjacencyList.at(m_currentTile).push_back(t);

		if (m_graph.adjacencyList.find(t) == m_graph.adjacencyList.end())
		{
			m_graph.adjacencyList[t] = {};
		}

		m_currentTile = t;

		return true;
	}

	return false;
}


void Drone::explore(Direction previous)
{
	for (int di = 1; di <= 4; ++di)
	{
		const auto d = static_cast<Direction>(di);
		const auto t = nextTile(m_currentTile, d);
		if (m_graph.tileMap.find(t) != m_graph.tileMap.end())
		{
			continue;
		}

		if (advance(d))
		{
			explore(d);
		}
	}

	if (previous != Still)
	{
		advance(inverse(previous));
	}
}


void Drone::stop()
{
	m_input.stop();
	m_output.stop();
	m_thread.join();
}


Graph Drone::buildGraph(const std::string & filename)
{
	Drone dr;
	dr.start(filename);

	dr.explore();
	dr.stop();

	return dr.m_graph;
}

DistanceMap bfs(const Graph & g, Tile start)
{
	DistanceMap dist;

	for(const auto & e : g.adjacencyList)
	{
		dist[e.first] = -1;
	}

	std::queue<std::pair<Tile, int>> queue;

	queue.push(std::make_pair(start, 0));

	while(!queue.empty())
	{
		const auto tile = queue.front();
		queue.pop();

		dist[tile.first] = tile.second;

		for(const auto & adj : g.adjacencyList.at(tile.first))
		{
			if(dist.at(adj) == -1)
			{
				queue.push(std::make_pair(adj, tile.second + 1));
			}
		}
	}

	return dist;
}

int distStartToOxygen(const Graph & g)
{
	return bfs(g, g.start).at(g.oxygen);
}

int oxygenDistributionTime(const Graph & g)
{
	const auto dist = bfs(g, g.oxygen);
	return std::max_element(dist.begin(), dist.end(), [](const std::pair<Tile, int> & a, const std::pair<Tile, int> & b){
		return a.second < b.second;
	})->second;
}
