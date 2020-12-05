#pragma once

#include <map>
#include <list>
#include <string>
#include <thread>

#include <Eigen/Core>

#include "Intprogram.h"
#include "InterProgramCommunication.h"

#include "n_comparator.h"
#include "find_optional.h"

using Tile = Eigen::Vector2i;
using TileComparator = NComparator<Tile, 2>;

class Image
{
public:
	Image(std::vector<char> data, int width);

	char at(int row, int col) const;
	char & at(int row, int col);

	char at(const Tile & t) const { return at(t(0), t(1)); }
	char & at(const Tile & t) { return at(t(0), t(1)); }

	bool isInside(int row, int col) const { return row >= 0 && row < m_height && col >= 0 && col < m_width;}
	bool isInside(const Tile & t) const { return isInside(t(0), t(1)); }

	std::string display() const;

	auto rows() const { return m_height; }
	auto cols() const { return m_width; }

private:
	int m_width, m_height;
	std::vector<char> m_data;
};

Image fromProgram(std::vector<Integer> code);
std::vector<std::tuple<int, int>> getIntersections(const Image & img);
Image markIntersections(Image img,const std::vector<std::tuple<int, int>> & inter);
int getAlignmentParameter(const Image & img);

enum Direction
{
	North = 1,
	South = 2,
	West = 3,
	East = 4
};


class PathBuilder
{
public:
	static std::string buildPath(const Image & image);

private:
	PathBuilder(const Image & img);

	void build(const Tile & start, Direction d);
	bool advance();

private:
	Tile m_currentPos;
	Direction m_dir;
	std::string m_path = "";
	Image m_img;
};

struct CompressedPath{
	std::string A, B, C, S;
};

std::optional<CompressedPath> trySplit(std::string s, int lenA, int lenB, int lenC);
CompressedPath getCompressedPath(const std::string& s);

Integer dust(const std::vector<Integer> & code, const CompressedPath& path);