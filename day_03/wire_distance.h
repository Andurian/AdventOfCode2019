#pragma once

#include <iostream>

#include "read_file.h"

struct Point
{
	int x, y;

	bool operator==(const Point & other) const { return x == other.x && y == other.y; }
	bool operator!=(const Point & other) const { return !(*this == other); }

	bool operator<(const Point & other) const
	{
		return x == other.x ? y < other.y : x < other.x;
	}
};


std::ostream & operator<<(std::ostream & os, const Point & p);


std::vector<Point> wire_corners(const std::string & directions);

std::function<Point(const Point &)> direction_functor(char direction);
std::vector<Point> wire_tiles(const Point & initialTile, const std::string & direction);
std::vector<Point> wire_tiles(const std::string & description);
std::vector<Point> wire_intersections(const std::string & wire1, const std::string & wire2);
std::vector<Point> wire_intersections(std::vector<Point> wire1, std::vector<Point> wire2);

int closest_intersection(const std::string & wire1, const std::string & wire2);
size_t closest_intersection_by_steps(const std::string & wire1, const std::string & wire2);
