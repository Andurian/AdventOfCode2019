#include "keylab.h"

#include <cctype>
#include <iostream>
#include <queue>
#include <set>

#include "DistanceFunction.h"


namespace
{
	struct Entry
	{
		Tile tile;
		int distance;
		std::set<char> keys;
	};


	struct EntryPriorityComparator
	{
		bool operator()(const Entry & a, const Entry & b) const { return a.distance > b.distance; }
	};


	struct EntryNoDistance
	{
		Tile tile;
		std::set<char> keys;
	};


	struct EntryNoDistanceComparator
	{
		bool operator()(const EntryNoDistance & a, const EntryNoDistance & b) const
		{
			if (a.keys == b.keys)
			{
				return NComparator<Tile, 2>{ }(a.tile, b.tile);
			}
			return a.keys < b.keys;
		}
	};


	std::ostream & operator<<(std::ostream & os, const Entry & e)
	{
		os << "(" << e.tile(0) << ", " << e.tile(1) << ") - {";

		bool first = true;
		for (const auto c : e.keys)
		{
			if (!first)
			{
				os << ",";
			}
			else
			{
				first = false;
			}
			os << c;
		}

		os << "} - " << e.distance;

		return os;
	}


	std::ostream & operator<<(std::ostream & os, const EntryNoDistance & e)
	{
		os << "(" << e.tile(0) << ", " << e.tile(1) << ") - {";

		bool first = true;
		for (const auto c : e.keys)
		{
			if (!first)
			{
				os << ",";
			}
			else
			{
				first = false;
			}
			os << c;
		}

		os << "}";

		return os;
	}


	using EntryPriorityQueue = std::priority_queue<Entry, std::vector<Entry>, EntryPriorityComparator>;
} // namespace


int anotherPathFindingAttempt(const Graph & g, const Tile & start)
{
	auto distFunc = KeyDistanceFunction(g, start);

	std::set<char> targetKeys;
	for (const auto it : g.poiMap)
	{
		if (std::islower(it.first))
		{
			targetKeys.insert(it.first);
		}
	}

	EntryPriorityQueue queue;
	std::map<EntryNoDistance, int, EntryNoDistanceComparator> visited;

	queue.push({ start, 0, std::set<char>{ } });
	visited[{ start, std::set<char>{ } }] = 0;

	int mindist = std::numeric_limits<int>::max();

	while (!queue.empty())
	{
		const auto entry = queue.top();
		queue.pop();

		// We found a better way to reach this tile
		if (visited.at({ entry.tile, entry.keys }) < entry.distance)
		{
			continue;
		}

		// std::cout << "Look at: " << entry << std::endl;

		if (entry.keys == targetKeys)
		{
			std::cout << "DONE at " << entry;
			if (entry.distance < mindist)
			{
				mindist = entry.distance;
				std::cout << " (new min)";
			}
			std::cout << std::endl;
		}

		const auto reachable = distFunc.distances(entry.tile, entry.keys);

		// std::cout << "Reachable: " << std::endl;
		for (const auto & dist_entry : reachable)
		{
			char targetKey = g.tileMap.at(dist_entry.first);

			// We already were at that target
			if (entry.keys.find(targetKey) != entry.keys.end())
			{
				continue;
			}

			const int aboutToVisitDistance = entry.distance + dist_entry.second;
			if (aboutToVisitDistance > mindist)
			{
				continue;
			}

			// std::cout << "\tTile: (" << dist_entry.first(0) << ", " << dist_entry.first(1) << ") - " << dist_entry.second <<
			// std::endl;

			auto keysCopy = entry.keys;
			keysCopy.insert(targetKey);

			EntryNoDistance aboutToVisit{ dist_entry.first, keysCopy };

			// std::cout << "\t" << aboutToVisit;
			const auto it = visited.find(aboutToVisit);
			if (it == visited.end() || it->second > aboutToVisitDistance)
			{
				// std::cout << " OK";
				visited[aboutToVisit] = aboutToVisitDistance;
				queue.push({ dist_entry.first, aboutToVisitDistance, keysCopy });
			}
			else
			{
				// std::cout << " NO";
			}
			// std::cout << std::endl;
		}
	}
	return mindist;
}


namespace
{
	struct MultiEntry
	{
		std::vector<Tile> tiles;
		int distance;
		std::set<char> keys;
	};


	struct MultiEntryPriorityComparator
	{
		bool operator()(const MultiEntry & a, const MultiEntry & b) const { return a.distance > b.distance; }
	};


	struct MultiEntryNoDistance
	{
		std::vector<Tile> tiles;
		std::set<char> keys;
	};


	struct MultiEntryNoDistanceComparator
	{
		bool operator()(const MultiEntryNoDistance & a, const MultiEntryNoDistance & b) const
		{
			if (a.keys == b.keys)
			{
				for(size_t i = 0; i < a.tiles.size(); ++i)
				{
					if(a.tiles[i] == b.tiles[i])
					{
						continue;
					}
					return NComparator<Tile, 2>{ }(a.tiles[i], b.tiles[i]);
				}
			}
			return a.keys < b.keys;
		}
	};


	std::ostream & operator<<(std::ostream & os, const MultiEntry & e)
	{
		os << "[";

		bool first = true;
		for (const auto & t : e.tiles)
		{
			if(!first)
			{
				os << ", ";
			}
			else
			{
				first = false;
			}
			os << "(" << t(0) << ", " << t(1) << ")";
		}
		
		os << "] - {";

		first = true;
		for (const auto c : e.keys)
		{
			if (!first)
			{
				os << ",";
			}
			else
			{
				first = false;
			}
			os << c;
		}

		os << "} - " << e.distance;

		return os;
	}


	std::ostream & operator<<(std::ostream & os, const MultiEntryNoDistance & e)
	{
		os << "[";

		bool first = true;
		for (const auto & t : e.tiles)
		{
			if (!first)
			{
				os << ", ";
			}
			else
			{
				first = false;
			}
			os << "(" << t(0) << ", " << t(1) << ")";
		}

		os << "] - {";

		first = true;
		for (const auto c : e.keys)
		{
			if (!first)
			{
				os << ",";
			}
			else
			{
				first = false;
			}
			os << c;
		}

		os << "}";

		return os;
	}


	using MultiEntryPriorityQueue = std::priority_queue<MultiEntry, std::vector<MultiEntry>, MultiEntryPriorityComparator>;
} // namespace


int anotherPathFindingAttempt(const Graph & g, const std::vector<Tile> & start)
{
	auto distFunc = KeyDistanceFunction(g, start);

	std::set<char> targetKeys;
	for (const auto it : g.poiMap)
	{
		if (std::islower(it.first))
		{
			targetKeys.insert(it.first);
		}
	}

	MultiEntryPriorityQueue queue;
	std::map<MultiEntryNoDistance, int, MultiEntryNoDistanceComparator> visited;

	queue.push({ start, 0, std::set<char>{ } });
	visited[{ start, std::set<char>{ } }] = 0;

	int mindist = std::numeric_limits<int>::max();

	while (!queue.empty())
	{
		const auto entry = queue.top();
		queue.pop();

		// We found a better way to reach this tile
		if (visited.at({ entry.tiles, entry.keys }) < entry.distance)
		{
			continue;
		}

		// std::cout << "Look at: " << entry << std::endl;

		if (entry.keys == targetKeys)
		{
			std::cout << "DONE at " << entry;
			if (entry.distance < mindist)
			{
				mindist = entry.distance;
				std::cout << " (new min)";
			}
			std::cout << std::endl;
		}

		for (size_t i = 0; i< entry.tiles.size(); ++i)
		{
			const auto currentTile = entry.tiles[i];

			const auto reachable = distFunc.distances(currentTile, entry.keys);

			// std::cout << "Reachable: " << std::endl;
			for (const auto & dist_entry : reachable)
			{
				char targetKey = g.tileMap.at(dist_entry.first);

				// We already were at that target
				if (entry.keys.find(targetKey) != entry.keys.end())
				{
					continue;
				}

				const int aboutToVisitDistance = entry.distance + dist_entry.second;
				if (aboutToVisitDistance > mindist)
				{
					continue;
				}

				// std::cout << "\tTile: (" << dist_entry.first(0) << ", " << dist_entry.first(1) << ") - " << dist_entry.second <<
				// std::endl;

				auto keysCopy = entry.keys;
				keysCopy.insert(targetKey);

				auto tilesCpy = entry.tiles;
				tilesCpy[i] = dist_entry.first;

				MultiEntryNoDistance aboutToVisit{ tilesCpy, keysCopy };

				// std::cout << "\t" << aboutToVisit;
				const auto it = visited.find(aboutToVisit);
				if (it == visited.end() || it->second > aboutToVisitDistance)
				{
					// std::cout << " OK";
					visited[aboutToVisit] = aboutToVisitDistance;
					queue.push({ tilesCpy, aboutToVisitDistance, keysCopy });
				}
				else
				{
					// std::cout << " NO";
				}
				// std::cout << std::endl;
			}
		}
	}
	return mindist;
}
