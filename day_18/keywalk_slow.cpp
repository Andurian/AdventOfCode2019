#include "keywalk_slow.h"

#include <cctype>

namespace
{
	struct State
	{
		Tile tile;
		std::set<char> keys;
	};

	struct StateComparator
	{
		bool operator()(const State & a, const State & b)
		{
			if (a.keys == b.keys)
			{
				return TileComparator{}(a.tile, b.tile);
			}

			return a.keys < b.keys;
		}
	};
} // namespace


int keyWalk(const Graph & g, Tile start, std::set<char> targetKeys)
{
	long long itCounter = 0;
	long long currentMinKeyLength = 0;

	std::map<Tile, std::set<std::set<char>>, TileComparator> visitedStates;
	for (const auto x : g.tileMap)
	{
		visitedStates[x.first] = {};
	}
	std::list<std::pair<State, int>> queue;

	queue.push_back(std::make_pair(State{ start, {} }, 0));

	while (!queue.empty())
	{
		const auto p = queue.front();
		queue.pop_front();

		const auto tile = p.first.tile;
		auto keys = p.first.keys;
		const auto len = p.second;


		visitedStates.at(tile).insert(keys);

		const auto c = g.tileMap.at(tile);

		// Pick up key
		if (std::islower(c))
		{
			keys.insert(c);
			// printCommaSeparatedLine(keys);
			if (keys == targetKeys)
			{
				return len;
			}
		}

		// Stuck at closed door. Abandon path
		if (std::isupper(c) && keys.find(std::tolower(c)) == keys.end())
		{
			continue;
		}

		for (const auto & adj : g.adjacencyList.at(tile))
		{
			State nextState{ adj, keys };
			const auto visited = visitedStates.at(adj);
			if (visited.find(keys) == visited.end())
			{
				queue.push_back(std::make_pair(nextState, len + 1));
			}
		}
	}

	return -1;
}


int keyWalkImage(const Image & img)
{
	const auto start = img.findFirst('@');
	const auto g = GraphBuilder::buildGraph(img, start);
	std::set<char> keys;
	for (const auto it : g.poiMap)
	{
		if (std::islower(it.first))
		{
			keys.insert(it.first);
		}
	}
	return keyWalk(g, start, keys);
}