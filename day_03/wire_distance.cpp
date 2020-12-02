#include "wire_distance.h"

#include "read_file.h"

namespace
{
	std::function<Point(const Point &, int)> getPointModifier(char c)
	{
		switch(c)
		{
			case 'U': return [](const Point & p, int x) { return Point{ p.x, p.y + x }; };
			case 'D': return [](const Point & p, int x) { return Point{ p.x, p.y - x }; };
			case 'L': return [](const Point & p, int x) { return Point{ p.x - x, p.y }; };
			case 'R': return [](const Point & p, int x) { return Point{ p.x + x, p.y }; };
			default: throw std::runtime_error("Invalid Direction Identifier");
		}
	}
}

std::ostream & operator<<(std::ostream & os, const Point & p)
{
	return os << "[" << p.x << ", " << p.y << "]";
}


std::vector<Point> wire_corners(const std::string & directions)
{
	const auto vecDirections = split_string(directions, ",");

	std::vector<Point> ret{ { 0, 0 } };

	for(const auto d : vecDirections)
	{
		ret.push_back(getPointModifier(d[0])(ret.back(), convert<int>(d.substr(1))));
	}

	return ret;
}


std::function<Point(const Point &)> direction_functor(char direction)
{
	switch(direction)
	{
		case 'R': return [](const Point & p) { return Point{ p.x + 1, p.y }; };
		case 'U': return [](const Point & p) { return Point{ p.x, p.y + 1 }; };
		case 'L': return [](const Point & p) { return Point{ p.x - 1, p.y }; };
		case 'D': return [](const Point & p) { return Point{ p.x, p.y - 1 }; };
		default: return [](const Point & p) { return p; };
	}
}


std::vector<Point> wire_tiles(const Point & initialTile, const std::string & direction)
{
	auto func = direction_functor(direction[0]);
	auto num = convert<int>(direction.substr(1));

	if(num == 0)
	{
		return {};
	}

	std::vector<Point> ret{func(initialTile)};
	ret.reserve(num);

	for(int i = 1; i < num; ++i)
	{
		ret.push_back(func(ret.back()));
	}

	return ret;
}


std::vector<Point> wire_tiles(const std::string & description)
{
	const auto instructions = split_string(description, ",");;

	std::vector<Point> wire{ Point{0, 0} };

	for(const auto & i : instructions)
	{
		std::vector<Point> segment = wire_tiles(wire.back(), i);
		for(const auto & p : segment)
		{
			wire.push_back(p);
		}
	}

	wire.erase(wire.begin());

	return wire;
}


std::vector<Point> wire_intersections(const std::string & wire1, const std::string & wire2)
{
	return wire_intersections(wire_tiles(wire1), wire_tiles(wire2));
}


std::vector<Point> wire_intersections(std::vector<Point> wire1, std::vector<Point> wire2)
{
	std::sort(wire1.begin(), wire1.end());
	std::sort(wire2.begin(), wire2.end());

	std::vector<Point> crossings;
	std::set_intersection(wire1.begin(), wire1.end(), wire2.begin(), wire2.end(), std::back_inserter(crossings));

	std::sort(crossings.begin(), crossings.end());

	return crossings;
}


int closest_intersection(const std::string & wire1, const std::string & wire2)
{
	std::vector<Point> intersections = wire_intersections(wire1, wire2);
	std::vector<int> distances;

	std::transform(intersections.begin(), intersections.end(), std::back_inserter(distances),
		[](const Point & p) { return std::abs(p.x) + std::abs(p.y); });

	return *std::min_element(distances.begin(), distances.end());
}


size_t closest_intersection_by_steps(const std::string & wire1, const std::string & wire2)
{
	auto tiles1 = wire_tiles(wire1);
	auto tiles2 = wire_tiles(wire2);

	auto intersections = wire_intersections(tiles1, tiles2);
	std::vector<size_t> distances;

	std::transform(intersections.begin(), intersections.end(), std::back_inserter(distances), 
		[&tiles1, &tiles2](const Point & i)
	{
		auto it1 = std::find(tiles1.begin(), tiles1.end(), i);
		auto it2 = std::find(tiles2.begin(), tiles2.end(), i);

		return std::distance(tiles1.begin(), it1) + std::distance(tiles2.begin(), it2) + 2;
	});

	return *std::min_element(distances.begin(), distances.end());
}
