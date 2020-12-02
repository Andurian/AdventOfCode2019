#include "DistanceFunction.h"

#include <queue>
#include <iostream>
#include <cctype>


KeyDistanceFunction::KeyDistanceFunction(const Graph & g, const Tile & start)
	: KeyDistanceFunction{ g, std::vector<Tile>{ start } }
{
	// empty
}


KeyDistanceFunction::KeyDistanceFunction(const Graph & g, const std::vector<Tile> & start)
	: m_graph{ g }
{
	for (const auto & s : start)
	{
		m_map[s] = bfs_keybound(g, s);
	}

	for (const auto it : g.poiMap)
	{
		if (std::islower(it.first))
		{
			m_map[it.second] = bfs_keybound(g, it.second);
		}
	}
}


int KeyDistanceFunction::distance(const Tile & source, const Tile & target, const std::set<char> keys)
{
	const auto entry = m_map.at(source).at(target);

	if (std::includes(keys.begin(), keys.end(), entry.keysNeeded.begin(), entry.keysNeeded.end()))
	{
		return entry.tiles;
	}
	return std::numeric_limits<int>::max();
}


int KeyDistanceFunction::distance(char sourceKey, char targetKey, const std::set<char> keys)
{
	return distance(m_graph.poiMap.at(sourceKey), m_graph.poiMap.at(targetKey), keys);
}


std::map<Tile, int, TileComparator> KeyDistanceFunction::distances(const Tile & source, const std::set<char> keys)
{
	std::map<Tile, int, TileComparator> ret;

	for (const auto & entry : m_map.at(source))
	{
		if (std::includes(keys.begin(), keys.end(), entry.second.keysNeeded.begin(), entry.second.keysNeeded.end()))
		{
			ret[entry.first] = entry.second.tiles;
		}
	}

	return ret;
}


std::map<Tile, int, TileComparator> KeyDistanceFunction::distances(char sourceKey, const std::set<char> keys)
{
	return distances(m_graph.poiMap.at(sourceKey), keys);
}


DistanceMap bfs_keybound(const Graph & g, const Tile & start)
{
	DistanceMap distKeys;
	DistanceMap dist;

	for (const auto & e : g.adjacencyList)
	{
		dist[e.first] = { -1, { } };
	}

	std::queue<std::tuple<Tile, int, std::set<char>>> queue;

	queue.push(std::make_tuple(start, 0, std::set<char>{ }));

	while (!queue.empty())
	{
		const auto tup = queue.front();
		queue.pop();

		const auto tile = std::get<0>(tup);
		const auto d = std::get<1>(tup);
		auto keys = std::get<2>(tup);

		char c = g.tileMap.at(tile);
		if (std::isupper(c))
		{
			keys.insert(std::tolower(c));
		}

		if (std::islower(c))
		{
			distKeys[tile] = { d, keys };
		}

		dist[tile] = { d, keys };

		for (const auto & adj : g.adjacencyList.at(tile))
		{
			if (dist.at(adj).tiles == -1)
			{
				queue.push(std::make_tuple(adj, d + 1, keys));
			}
		}
	}

	return distKeys;
}
