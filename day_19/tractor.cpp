#include "tractor.h"

#include "console_image.h"
#include "read_file.h"

#include <iostream>

Image::Image(int width, int height, char fill) : m_width{ width }, m_height{ height }, m_data(width * height, fill)
{
	// empty
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


std::string Image::draw() const
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

void drawBox(int x, int y, int width, int height, Image & img)
{
    for(int py = y; py < y + height; ++py)
    {
        for(int px = x; px < x + width; ++px)
        {
            img.at(px, py) = '0';
        }
    }
}

bool isInsideBeam(int x, int y, const std::vector<Integer> & code)
{
	InterProgramCommunication in, out;

	std::thread t{ [&] {
		Intprogram prog{ in, out, code };
		prog.run();
	} };

	in.write(x);
	in.write(y);

	const auto result = out.readNext();

	in.stop();
	out.stop();
	t.join();

	return result;
}


Image imageFromTractorCode(int width, int height, const std::vector<Integer> & code)
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


int countTractorPoints(int width, int height, const std::vector<Integer> & code)
{
    int count = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            count += isInsideBeam(x, y, code) ? 1 : 0;
        }
    }
    return count;
}


std::pair<int, int> findClosestPoint(int width, int height, const std::vector<Integer> & code)
{
    // My beam is so narrow that it has a gap an a few lines and columns without a beam
    // Search heuristic does not work for those areas
    // Solid beam starts at these coordinates
    int x = 5;
    int y = 4;

    while(true)
    {
        if(isInsideBeam(x, y, code))
        {
            // If the box does not fit horizontally we have to move lower
            if(!isInsideBeam(x + width - 1, y, code))
            {
                ++y;
                continue;
            }

            // If the box does not fit vertically we have to move further right
            if(!isInsideBeam(x, y + height - 1, code))
            {
                ++x;
                continue;
            }

            return {x, y};
        }

        // Initial point is no longer on the beam
        // Search current row and column for the closest point inside the beam
        int testOffset = 1;
        while(true)
        {
            if(isInsideBeam(x + testOffset, y, code))
            {
                x += testOffset;
                break;
            }

            if(isInsideBeam(x, y + testOffset, code))
            {
                y += testOffset;
                break;
            }

            ++testOffset;
        }
    }
}