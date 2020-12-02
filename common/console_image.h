#pragma once

#include <limits>
#include <map>
#include <string>

#include <Eigen/Core>

#include "n_comparator.h"

using Point = Eigen::Vector2i;
using PointComparator = NComparator<Point, 2>;

template<typename T>
using PointMap = std::map<Point, T, PointComparator>;

template<typename T>
using CharMapping = std::map<T, char>;

template<typename T>
struct ImageLimits
{
	T width() const { return x_max - x_min + 1; }
	T height() const { return y_max - x_min + 1; }

	T x_min = std::numeric_limits<T>::max();
	T x_max = std::numeric_limits<T>::lowest();

	T y_min = std::numeric_limits<T>::max();
	T y_max = std::numeric_limits<T>::lowest();
};

template<typename T>
ImageLimits<int> makeLimits(const PointMap<T> & m)
{
	ImageLimits<int> ret;

	for (const auto & it : m)
	{
		const auto & tile = it.first;

		ret.x_min = std::min(tile.x(), ret.x_min);
		ret.x_max = std::max(tile.x(), ret.x_max);

		ret.y_min = std::min(tile.y(), ret.y_min);
		ret.y_max = std::max(tile.y(), ret.y_max);
	}

	return ret;
}

std::string generate_image(const PointMap<char> & map, char empty, bool flip_xy = false);

template<typename T>
std::string generate_image_with_map(const PointMap<T> & map, const CharMapping<T> & charMapping, char empty)
{
	const auto limits = makeLimits(map);

	std::string img;
	for (int i = 0; i < limits.height(); ++i)
	{
		img += std::string(limits.width(), empty) + "\n";;
	}

	for (const auto & it : map)
	{
		const auto & tile = it.first;
		const auto & color = it.second;

		const auto offset = (tile.y() - limits.y_min) * (limits.width() + 1) + (tile.x() - limits.x_min);

		img[offset] = charMapping.at(color);
	}

	return img;
}