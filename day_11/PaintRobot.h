#pragma once

#include <set>
#include <map>

#include <Eigen/Core>

using Position = Eigen::Vector2i;


struct PositionComparator
{
	bool operator()(const Position & a, const Position & b) const
	{
		if (a.x() == b.x())
		{
			return a.y() < b.y();
		}

		return a.x() < b.x();
	}
};


enum Color { Black = 0, White = 1 };
enum class Turn { Left = 0, Right = 1 };
enum class Direction { Top = 0, Left = 1, Down = 2, Right = 3 };


using PositionSet = std::set<Position, PositionComparator>;
using PositionMap = std::map<Position, Color, PositionComparator>;


class PaintRobot
{
public:
	PaintRobot(Color startTile = Black);

	Color process(Color color, Turn turn);
	Color currentColor();

	const auto & getTiles() const { return m_tiles; }

	std::string getImage() const;
private:
	void paint(Color color);
	void turn(Turn turn);
	void move();
	

private:
	Position m_currentPosition;
	Direction m_currentDirection;

	PositionMap m_tiles;
};
