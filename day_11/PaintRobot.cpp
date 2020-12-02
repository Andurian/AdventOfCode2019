#include "PaintRobot.h"


namespace
{
	Direction turnLeft(Direction d)
	{
		switch (d)
		{
			case Direction::Top:
				return Direction::Left;
			case Direction::Left:
				return Direction::Down;
			case Direction::Down:
				return Direction::Right;
			case Direction::Right:
				return Direction::Top;
			default:
				throw std::runtime_error("Invalid Direction");
		}
	}


	Direction turnRight(Direction d)
	{
		switch (d)
		{
			case Direction::Top:
				return Direction::Right;
			case Direction::Left:
				return Direction::Top;
			case Direction::Down:
				return Direction::Left;
			case Direction::Right:
				return Direction::Down;
			default:
				throw std::runtime_error("Invalid Direction");
		}
	}


	Position move(Position p, Direction d)
	{
		switch (d)
		{
			case Direction::Top:
				return p + Position{ 0, -1 };
			case Direction::Left:
				return p + Position{ -1, 0 };
			case Direction::Down:
				return p + Position{ 0, 1 };
			case Direction::Right:
				return p + Position{ 1, 0 };
			default:
				throw std::runtime_error("Invalid Direction");
		}
	}
} // namespace


PaintRobot::PaintRobot(Color startTile) : m_currentPosition{ 0, 0 }, m_currentDirection{ Direction::Top }
{
	if (startTile == White)
	{
		paint(White);
	}
}


Color PaintRobot::process(Color color, Turn t)
{
	paint(color);
	turn(t);
	move();

	return currentColor();
}


std::string PaintRobot::getImage() const
{
	int x_min = std::numeric_limits<int>::max();
	int x_max = std::numeric_limits<int>::lowest();

	int y_min = std::numeric_limits<int>::max();
	int y_max = std::numeric_limits<int>::lowest();

	for (const auto & it : m_tiles)
	{
		const auto & tile = it.first;
		x_min = std::min(tile.x(), x_min);
		x_max = std::max(tile.x(), x_max);

		y_min = std::min(tile.y(), y_min);
		y_max = std::max(tile.y(), y_max);
	}

	const int w = x_max - x_min + 1;
	const int h = y_max - y_min + 1;

	auto line = std::string(w, '.') + "\n";
	std::string img;
	for (int i = 0; i < h; ++i)
	{
		img += line;
	}


	for (const auto & it : m_tiles)
	{
		const auto & tile = it.first;
		const auto & color = it.second;
		const auto offset = (tile.y() - y_min) * (w + 1) + (tile.x() - x_min);
		img[offset] = color == White ? '#' : ' ';
	}

	return img;
}


void PaintRobot::paint(Color color)
{
	m_tiles[m_currentPosition] = color;
}


void PaintRobot::turn(Turn turn)
{
	switch (turn)
	{
		case Turn::Left:
			m_currentDirection = turnLeft(m_currentDirection);
			return;
		case Turn::Right:
			m_currentDirection = turnRight(m_currentDirection);
			return;
		default:
			return;
	}
}


void PaintRobot::move()
{
	m_currentPosition = ::move(m_currentPosition, m_currentDirection);
}

Color PaintRobot::currentColor()
{
	const auto it = m_tiles.find(m_currentPosition);
	return it == m_tiles.end() ? Black : it->second;
}
