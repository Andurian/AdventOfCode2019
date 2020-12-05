#include "tractor.h"

#include "console_image.h"
#include "read_file.h"

#include <iostream>
#include <thread>


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
    // Find "real" start of the contiguous beam
    int x = 0;
    int y = 0;

	for(int tx = 1; tx < 10; ++tx)
	{
		for(int ty = 1; ty < 10; ++ty)
		{
			if(isInsideBeam(tx, ty, code)){
				x = tx;
				y = ty;
				goto offsetFound;
			}
		}
	}

	offsetFound:

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