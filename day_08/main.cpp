#include <algorithm>
#include <infix_ostream_iterator.h>

#include "read_file.h"
#include "space_image.h"
#include "multicall.h"

int taskA(const std::vector<int> & image, int width, int height)
{
	std::vector<std::vector<int>> layers = split_layers(image, width, height);

	std::vector<int> num0s;

	std::transform(layers.begin(), layers.end(), std::back_inserter(num0s), [](const std::vector<int> & layer)
	{
	  return static_cast<int>(std::count(layer.begin(), layer.end(), 0));
	});

	const auto it = layers.begin() + std::distance(num0s.begin(), std::min_element(num0s.begin(), num0s.end()));

	return static_cast<int>(std::count(it->begin(), it->end(), 1) * std::count(it->begin(), it->end(), 2));
}

void solve(const std::string& filename)
{
	const int width = 25;
	const int height = 6;

	std::vector<int> image = read_collection(filename, Converter<int>{});

	std::cout << "Result A: " << taskA(image, width, height) << std::endl;
	std::cout << "Image:\n" << to_string(image, width, height) << std::endl;
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
