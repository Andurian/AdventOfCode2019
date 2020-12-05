#pragma once

#include <string>
#include <vector>

#include "Tile.h"

class Image
{
public:
	Image(std::vector<char> data, int width);

	Tile findFirst(char c) const;
	std::vector<Tile> findAll(char c) const;

	char at(int row, int col) const;
	char & at(int row, int col);

	char at(const Tile & t) const { return at(t(0), t(1)); }
	char & at(const Tile & t) { return at(t(0), t(1)); }

	bool isInside(int row, int col) const { return row >= 0 && row < m_height && col >= 0 && col < m_width; }
	bool isInside(const Tile & t) const { return isInside(t(0), t(1)); }

	std::string display() const;

	auto rows() const { return m_height; }
	auto cols() const { return m_width; }

	static Image fromFile(const std::string & filename);

private:
	int m_width, m_height;
	std::vector<char> m_data;
};