#pragma once

#include <map>
#include <list>
#include <string>
#include <thread>

#include <Eigen/Core>

#include "Intprogram.h"
#include "InterProgramCommunication.h"

#include "n_comparator.h"

using Tile = Eigen::Vector2i;
using TileComparator = NComparator<Tile, 2>;

class Image
{
public:
	Image(int width, int height, char fill = ' ');
	Image(std::vector<char> data, int width);

	char at(int x, int y) const;
	char & at(int x, int y);

	char at(const Tile & t) const { return at(t(0), t(1)); }
	char & at(const Tile & t) { return at(t(0), t(1)); }

	bool isInside(int x, int y) const { return y >= 0 && y < m_height && x >= 0 && x < m_width;}
	bool isInside(const Tile & t) const { return isInside(t(0), t(1)); }

	std::string draw() const;

	auto height() const { return m_height; }
	auto width() const { return m_width; }

private:
	int m_width, m_height;
	std::vector<char> m_data;
};

Image imageFromTractorCode(int width, int height, const std::vector<Integer> & code);
void drawBox(int x, int y, int width, int height, Image & img);

int countTractorPoints(int width, int height, const std::vector<Integer> & code);
std::pair<int, int> findClosestPoint(int width, int height, const std::vector<Integer> & code);



