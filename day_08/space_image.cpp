#include "space_image.h"

std::vector<std::vector<int>> split_layers(const std::vector<int> & image, int width, int height)
{
	const std::size_t numElements = width * height;
	std::vector<std::vector<int>> ret;

	auto it = image.begin();

	while (it != image.end())
	{
		ret.push_back(std::vector<int>{ it, it + numElements });
		std::advance(it, numElements);
	}

	return ret;
}

std::vector<int> final_color(const std::vector<int> & image, int width, int height)
{
	std::vector<std::vector<int>> layers = split_layers(image, width, height);
	std::vector<int> ret;
	ret.reserve(width * height);

	for (std::size_t pixel = 0; pixel < layers[0].size(); ++pixel)
	{
		for (std::size_t layer = 0; layer < layers.size(); ++layer)
		{
			int color = layers[layer][pixel];
			if (color != 2)
			{
				ret.push_back(color);
				break;
			}
		}
	}
	return ret;
}

std::string to_string(const std::vector<int> & image, int width, int height)
{
	std::vector<int> colors = final_color(image, width, height);

	std::string ret;

	for (std::size_t i = 0; i < colors.size(); ++i)
	{
		if (i > 0 && (i % width) == 0)
		{
			ret.push_back('\n');
		}
		if (colors[i] == 1)
		{
			ret.push_back('0');
		}
		else
		{
			ret.push_back(' ');
		}
	}
	return ret;
}
