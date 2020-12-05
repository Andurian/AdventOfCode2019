#include "Image.h"

#include <sstream>
#include <stdexcept>

#include "tractor.h"

Image::Image(int width, int height, char fill) : m_width{ width }, m_height{ height }, m_data(width * height, fill)
{
	// empty
}

void Image::drawBox(int x, int y, int width, int height)
{
	for (int py = y; py < y + height; ++py)
	{
		for (int px = x; px < x + width; ++px)
		{
			at(px, py) = '0';
		}
	}
}

Image::Image(std::vector<char> data, int width) : m_data{ std::move(data) }, m_width{ width }
{
	m_height = static_cast<int>(m_data.size()) / m_width;
	if (m_height * m_width != m_data.size())
	{
		throw std::runtime_error("");
	}
}


char Image::at(int x, int y) const
{
	return m_data.at(y * m_width + x);
}


char & Image::at(int x, int y)
{
	return m_data.at(y * m_width + x);
}


std::string Image::display() const
{
	std::stringstream ss;

	for (int x = 0; x < m_width; ++x)
	{
		ss << at(x, 0);
	}

	for (int y = 1; y < m_height; ++y)
	{
		ss << "\n";
		for (int x = 0; x < m_width; ++x)
		{
			ss << at(x, y);
		}
	}
	return ss.str();
}

Image Image::fromTractorCode(int width, int height, const Code& code)
{
	Image img{ width, height, '.' };

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const auto result = isInsideBeam(x, y, code);
			if (result == 1)
			{
				img.at(x, y) = '#';
			}
		}
	}
	return img;
}