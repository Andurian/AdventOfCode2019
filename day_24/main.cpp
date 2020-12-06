#include <iostream>
#include <map>
#include <set>
#include <thread>

#include "console_image.h"
#include "read_file.h"

class Field
{
public:
	enum Tile
	{
		Bug,
		Empty,
		Recursion
	};

	Tile at(Point p) const { return m_limits.isInside(p.x(), p.y()) ? m_field.at(p) : Empty; }
	Tile at(int x, int y) const { return at(Point{ x, y }); }

	std::string displayStr() const { return generate_image_with_map(m_field, getMapping()); }

	long long biodiversity() const
	{
		long long res = 0;
		long long factor = 1;

		for (int y = m_limits.y_min; y <= m_limits.y_max; ++y)
		{
			for (int x = m_limits.x_min; x <= m_limits.x_max; ++x)
			{
				if (at(x, y) == Bug)
				{
					res += factor;
				}

				factor <<= 1;
			}
		}

		return res;
	}

	bool hasBugsOnEdge() const
	{
		for (int y = m_limits.y_min; y <= m_limits.y_max; ++y)
		{
			for (int x = m_limits.x_min; x <= m_limits.x_max; ++x)
			{
				if (x != m_limits.x_min && x != m_limits.x_max && y != m_limits.y_min && y != m_limits.y_max)
				{
					continue;
				}

				if (at(x, y) == Bug)
				{
					return true;
				}
			}
		}
		return false;
	}

	void advance()
	{
		PointMap<Tile> newMap;

		for (int y = m_limits.y_min; y <= m_limits.y_max; ++y)
		{
			for (int x = m_limits.x_min; x <= m_limits.x_max; ++x)
			{
				int bugCnt = 0;

				if (at(x - 1, y) == Bug)
				{
					++bugCnt;
				}

				if (at(x + 1, y) == Bug)
				{
					++bugCnt;
				}

				if (at(x, y - 1) == Bug)
				{
					++bugCnt;
				}

				if (at(x, y + 1) == Bug)
				{
					++bugCnt;
				}

				if (at(x, y) == Bug)
				{
					if (bugCnt != 1)
					{
						newMap.insert(std::make_pair(Point{ x, y }, Empty));
					}
					else
					{
						newMap.insert(std::make_pair(Point{ x, y }, Bug));
					}
				}
				else
				{
					if (bugCnt == 1 || bugCnt == 2)
					{
						newMap.insert(std::make_pair(Point{ x, y }, Bug));
					}
					else
					{
						newMap.insert(std::make_pair(Point{ x, y }, Empty));
					}
				}
			}
		}

		m_field = newMap;
	}

	void set(int x, int y, Tile t) { m_field.at(Point{ x, y }) = t; }

	static Field fromString(const std::string & s)
	{
		PointMap<Tile> map;

		int x = 0;
		int y = 0;

		for (char c : s)
		{
			if (c == '\n')
			{
				x = 0;
				++y;
				continue;
			}

			if (c == '.')
			{
				map.insert(std::make_pair(Point{ x, y }, Empty));
			}
			else if (c == '#')
			{
				map.insert(std::make_pair(Point{ x, y }, Bug));
			}
			else if (c == '?')
			{
				map.insert(std::make_pair(Point{ x, y }, Recursion));
			}

			++x;
		}

		return Field{ std::move(map) };
	}

	static Field emptyRecursiveField()
	{
		PointMap<Tile> map;

		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 5; ++x)
			{
				if (y == 2 && x == 2)
				{
					map.insert(std::make_pair(Point{ x, y }, Recursion));
				}
				else
				{
					map.insert(std::make_pair(Point{ x, y }, Empty));
				}
			}
		}

		return Field{ std::move(map) };
	};

private:
	Field(PointMap<Tile> map) : m_field{ std::move(map) }, m_limits(makeLimits(m_field))
	{
		// empty
	}

	static CharMapping<Tile> getMapping() { return { { Bug, '#' }, { Empty, '.' }, { Recursion, '?' } }; }

	PointMap<Tile> m_field;
	ImageLimits<int> m_limits;
};

class MultiField
{
public:
	struct Index
	{
		int level, x, y;
	};

	MultiField(Field f) { m_fields.insert(std::make_pair(0, std::move(f))); }

	void advance()
	{
		int newMinLevel = m_fields.at(m_minLevel).hasBugsOnEdge() ? m_minLevel - 1 : m_minLevel;
		int newMaxLevel = m_fields.at(m_maxLevel).hasBugsOnEdge() ? m_maxLevel + 1 : m_maxLevel;

		std::map<int, Field> newFields;
		for (int i = newMinLevel; i <= newMaxLevel; ++i)
		{
			newFields.insert(std::make_pair(i, Field::emptyRecursiveField()));
			// std::cout << "FIELD ADDED: " << i << "\n" << newFields.at(i).displayStr() << std::endl;
		}

		for (int level = newMinLevel; level <= newMaxLevel; ++level)
		{
			for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 5; ++x)
				{
					if (at({ level, x, y }) == Field::Recursion)
					{
						newFields.at(level).set(x, y, Field::Recursion);
						continue;
					}

					int bugCnt = countBugs(neighbors(level, x, y));

					if (at({ level, x, y }) == Field::Bug)
					{
						if (bugCnt != 1)
						{
							newFields.at(level).set(x, y, Field::Empty);
						}
						else
						{
							newFields.at(level).set(x, y, Field::Bug);
						}
					}
					else if (at({ level, x, y }) == Field::Empty)
					{
						if (bugCnt == 1 || bugCnt == 2)
						{
							newFields.at(level).set(x, y, Field::Bug);
						}
						else
						{
							newFields.at(level).set(x, y, Field::Empty);
						}
					}
				}
			}
		}

		m_minLevel = newMinLevel;
		m_maxLevel = newMaxLevel;
		m_fields = newFields;
	}

	int countBugs(const std::vector<Index> & idx) const
	{
		int cnt = 0;
		for (const auto & i : idx)
		{
			if (at(i) == Field::Bug)
			{
				++cnt;
			}
		}

		return cnt;
	}

	int totalBugs()
	{
		int cnt = 0;
		for (int level = m_minLevel; level <= m_maxLevel; ++level)
		{
			for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 5; ++x)
				{
					if (at({ level, x, y }) == Field::Bug)
					{
						++cnt;
					}
				}
			}
		}
		return cnt;
	}

	Field::Tile at(Index i) const
	{
		if (i.x == 2 && i.y == 2)
		{
			return Field::Recursion;
		}

		if (i.level < m_minLevel || i.level > m_maxLevel)
		{
			return Field::Empty;
		}

		return m_fields.at(i.level).at(i.x, i.y);
	}

	void display()
	{
		for (int level = m_minLevel; level <= m_maxLevel; ++level)
		{
			std::cout << "Depth: " << level << std::endl;
			std::cout << m_fields.at(level).displayStr() << std::endl << std::endl;
		}
	}


private:
	static std::vector<Index> neighbors(int level, int x, int y)
	{
		std::vector<Index> ret;

		for (const auto i : topNeighbor(level, x, y))
		{
			ret.push_back(i);
		}

		for (const auto i : leftNeighbor(level, x, y))
		{
			ret.push_back(i);
		}

		for (const auto i : rightNeighbor(level, x, y))
		{
			ret.push_back(i);
		}

		for (const auto i : bottomNeighbor(level, x, y))
		{
			ret.push_back(i);
		}

		return ret;
	}

	static std::vector<Index> topNeighbor(int level, int x, int y)
	{
		if (y == 0)
		{
			return { Index{ level - 1, 2, 1 } };
		}

		if (y == 3 && x == 2)
		{
			std::vector<Index> ret;
			for (int innerX = 0; innerX < 5; ++innerX)
			{
				ret.push_back(Index{ level + 1, innerX, 4 });
			}
			return ret;
		}

		return { Index{ level, x, y - 1 } };
	}

	static std::vector<Index> bottomNeighbor(int level, int x, int y)
	{
		if (y == 1 && x == 2)
		{
			std::vector<Index> ret;
			for (int innerX = 0; innerX < 5; ++innerX)
			{
				ret.push_back(Index{ level + 1, innerX, 0 });
			}
			return ret;
		}
		if (y == 4)
		{
			return { Index{ level - 1, 2, 3 } };
		}

		return { Index{ level, x, y + 1 } };
	}

	static std::vector<Index> leftNeighbor(int level, int x, int y)
	{
		if (x == 0)
		{
			return { Index{ level - 1, 1, 2 } };
		}

		if (y == 2 && x == 3)
		{
			std::vector<Index> ret;
			for (int innerY = 0; innerY < 5; ++innerY)
			{
				ret.push_back(Index{ level + 1, 4, innerY });
			}
			return ret;
		}

		return { Index{ level, x - 1, y } };
	}

	static std::vector<Index> rightNeighbor(int level, int x, int y)
	{
		if (y == 2 && x == 1)
		{
			std::vector<Index> ret;
			for (int innerY = 0; innerY < 5; ++innerY)
			{
				ret.push_back(Index{ level + 1, 0, innerY });
			}
			return ret;
		}

		if (x == 4)
		{
			return { Index{ level - 1, 3, 2 } };
		}

		return { Index{ level, x + 1, y } };
	}

	int m_minLevel = 0;
	int m_maxLevel = 0;

	std::map<int, Field> m_fields;
};

long long findRecurringBiodiversity(const std::string & filename)
{
	Field f = Field::fromString(read_file("input.txt"));

	std::set<long long> seenBiodivs;
	seenBiodivs.insert(f.biodiversity());

	while (true)
	{
		f.advance();
		auto div = f.biodiversity();
		if (seenBiodivs.find(div) != seenBiodivs.end())
		{
			return div;
		}
		seenBiodivs.insert(div);
	}
}

void watchBugs(const std::string & filename, int iterations)
{
	Field f = Field::fromString(read_file("input.txt"));

	for (int i = 0; i < 200; ++i)
	{
		std::system("clear");
		f.advance();
		std::cout << f.displayStr() << std::endl;
		std::cout << "Biodiv: " << f.biodiversity() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds{ 400 });
	}
}

int countRecursiveBugs(const std::string & filename, int nIter)
{
	MultiField f{ Field::fromString(read_file(filename)) };

	for (int i = 0; i < nIter; ++i)
	{
		f.advance();
	}

	return f.totalBugs();
}

int main()
{
	std::cout << "Reucurring Biodiv: " << findRecurringBiodiversity("input.txt") << std::endl;
	std::cout << "Initial Bug Count = " << countRecursiveBugs("input_recursive.txt", 200) << std::endl;

	return 0;
}