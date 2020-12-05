#include <iostream>

#include "read_file.h"

#include "tractor.h"
#include "Image.h"
#include "multicall.h"

void sampleDisplay(const std::string& filename)
{
	const auto code = read_collection(filename, ",", Converter<Integer>{});

	auto img = Image::fromTractorCode(50, 50, code);

	const auto q = findClosestPoint(3, 3, code);
	img.drawBox(q.first, q.second, 3, 3);

	std::cout << img.display() << std::endl;
}

void solve(const std::string& filename)
{
	const auto code = read_collection(filename, ",", Converter<Integer>{});
	std::cout << "Points in 50 x 50 grid: " << countTractorPoints(50, 50, code) << std::endl;

	const auto p = findClosestPoint(100, 100, code);

	std::cout << "Closest point to fit 100 x 100 box: (" << p.first << ", " << p.second << ")" << std::endl;
	std::cout << "Result for second star: " << p.first * 10000 + p.second << std::endl;
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
