#include "Tile.h"


Tile nextTile(Tile t, Direction d)
{
	switch (d)
	{
		case North:
			return t + Tile{ -1, 0 };
		case South:
			return t + Tile{ 1, 0 };
		case West:
			return t + Tile{ 0, -1 };
		case East:
			return t + Tile{ 0, 1 };
		default:
			throw std::runtime_error("");
	}
}