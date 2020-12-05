#include "console_image.h"

std::string generate_image(const PointMap<char> & map, char empty, bool flip_xy)
{
	const auto limits = makeLimits(map);

	std::string img;

	if (flip_xy)
	{
		for (int i = 0; i < limits.width(); ++i)
		{
			img += std::string(limits.height(), empty) + "\n";
		}
	}
	else
	{
		for (int i = 0; i < limits.height(); ++i)
		{
			img += std::string(limits.width(), empty) + "\n";
		}
	}

	for (const auto & it : map)
	{
		const auto & tile = it.first;
		const auto & color = it.second;

		if (flip_xy)
		{
			const auto offset = (tile.x() - limits.x_min) * (limits.height() + 1) + (tile.y() - limits.y_min);

			img[offset] = color;
		}
		else
		{
			const auto offset = (tile.y() - limits.y_min) * (limits.width() + 1) + (tile.x() - limits.x_min);

			img[offset] = color;
		}
	}

	return img;
}