#include "Direction.h"

#include <stdexcept>


Direction nextDirectionRight(Direction d)
{
	switch (d)
	{
		case North:
			return East;
		case South:
			return West;
		case West:
			return North;
		case East:
			return South;
		default:
			throw std::runtime_error("");
	}
}


Direction nextDirectionLeft(Direction d)
{
	switch (d)
	{
		case North:
			return West;
		case South:
			return East;
		case West:
			return South;
		case East:
			return North;
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