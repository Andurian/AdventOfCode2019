#include "donut.h"

#include <iostream>
#include <queue>

using DistanceMap = std::map<Tile, int, TileComparator>;

int findPath(const Graph & g)
{
	DistanceMap dist;

	for (const auto & e : g.adjacencyList)
	{
		dist[e.first] = -1;
	}

	std::queue<std::tuple<Tile, int>> queue;

	queue.push(std::make_tuple(g.start, 0));

	while (!queue.empty())
	{
		const auto tup = queue.front();
		queue.pop();

		const auto tile = std::get<0>(tup);
		const auto d = std::get<1>(tup);

		dist[tile] = d;

		for (const auto & adj : g.adjacencyList.at(tile))
		{
			if (dist.at(adj) == -1)
			{
				queue.push(std::make_tuple(adj, d + 1));
			}
		}
	}

	return dist[g.end];
}

struct RecursiveDistance
{
	int steps;
	int level;
};
struct VisitedEntry
{
	Tile tile;
	int level;
};

struct VisitedEntryComparator
{
	bool operator()(const VisitedEntry & a, const VisitedEntry & b) const
	{
		if (a.level == b.level)
		{
			return TileComparator{}(a.tile, b.tile);
		}
		return a.level < b.level;
	}
};

int findRecursablePath(const Graph & g)
{
	std::queue<std::tuple<Tile, RecursiveDistance>> queue;
	queue.push(std::make_tuple(g.start, RecursiveDistance{ 0, 0 }));

	std::map<VisitedEntry, int, VisitedEntryComparator> visited;
	visited[{ g.start, 0 }] = 0;

	int mindist = std::numeric_limits<int>::max();

	while (!queue.empty())
	{
		const auto entry = queue.front();
		queue.pop();

		const auto tile = std::get<0>(entry);
		const auto dist = std::get<1>(entry);

		if (dist.steps > mindist)
		{
			continue;
		}

		// We found a better way to reach this tile
		if (visited.at({ tile, dist.level }) < dist.steps)
		{
			continue;
		}

		// std::cout << "Look at: " << entry << std::endl;

		if (dist.level == 0 && tile == g.end)
		{
			std::cout << "DONE at " << dist.steps;
			if (dist.steps < mindist)
			{
				mindist = dist.steps;
				std::cout << " (new min)";
			}
			std::cout << std::endl;
		}

		for (const auto & next : g.adjacencyList.at(tile))
		{
			VisitedEntry aboutToVisit{ next, dist.level };
			RecursiveDistance aboutToVisitDistance{ dist.steps + 1, dist.level };

			// std::cout << "\t" << aboutToVisit;
			const auto it = visited.find(aboutToVisit);
			if (it == visited.end() || it->second > aboutToVisitDistance.steps)
			{
				// std::cout << " OK";
				visited[aboutToVisit] = aboutToVisitDistance.steps;
				queue.push(std::make_tuple(next, aboutToVisitDistance));
			}
			// std::cout << std::endl;
		}

		auto change = g.portals.canPortal(tile);
		if (change != Portals::No)
		{
			const auto next = g.portals.portal(tile);
			const auto nextLevel = dist.level + static_cast<int>(change);

			if (nextLevel >= 0)
			{
				VisitedEntry aboutToVisit{ next, nextLevel };
				RecursiveDistance aboutToVisitDistance{ dist.steps + 1, nextLevel };

				// std::cout << "\t" << aboutToVisit;
				const auto it = visited.find(aboutToVisit);
				if (it == visited.end() || it->second > aboutToVisitDistance.steps)
				{
					// std::cout << " OK";
					visited[aboutToVisit] = aboutToVisitDistance.steps;
					queue.push(std::make_tuple(next, aboutToVisitDistance));
				}
			}
		}
	}
	return mindist;
}
