#pragma once

enum Direction
{
	Still = 0,
	North = 1,
	South = 2,
	West = 3,
	East = 4
};

Direction nextDirectionRight(Direction d);
Direction nextDirectionLeft(Direction d);
Direction inverse(Direction d);