#include "Image.h"

#include "read_file.h"

Image::Image(std::vector<char> data, int width) : m_data{ std::move(data) }, m_width{ width }
{
	m_height = static_cast<int>(m_data.size()) / m_width;
	if (m_height * m_width != m_data.size())
	{
		throw std::runtime_error("");
	}
}


char Image::at(int row, int col) const
{
	return m_data.at(row * m_width + col);
}


char & Image::at(int row, int col)
{
	return m_data.at(row * m_width + col);
}


std::string Image::display() const
{
	std::stringstream ss;

	for (int col = 0; col < m_width; ++col)
	{
		ss << at(0, col);
	}

	for (int row = 1; row < m_height; ++row)
	{
		ss << "\n";
		for (int col = 0; col < m_width; ++col)
		{
			ss << at(row, col);
		}
	}
	return ss.str();
}


Image Image::fromFile(const std::string & filename)
{
	const auto lines = read_collection(filename, "\n");

	const int width = static_cast<int>(lines.front().length());

	std::vector<char> data;
	data.reserve(lines.size() * width);
	for (const auto & l : lines)
	{
		for (const auto c : l)
		{
			data.push_back(c);
		}
	}

	return Image{ data, width };
}

Tile Image::findFirst(char c) const
{
	for (int row = 1; row < m_height; ++row)
	{
		for (int col = 0; col < m_width; ++col)
		{
			if (at(row, col) == c)
			{
				return { row, col };
			}
		}
	}
	return { -1, -1 };
}


std::vector<Tile> Image::findAll(char c) const
{
	std::vector<Tile> ret;
	for (int row = 1; row < m_height; ++row)
	{
		for (int col = 0; col < m_width; ++col)
		{
			if (at(row, col) == c)
			{
				ret.push_back({ row, col });
			}
		}
	}
	return ret;
}
