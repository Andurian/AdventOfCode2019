#include <iostream>

#include "ascii.h"
#include "read_file.h"
#include "multicall.h"


void solve(const std::string& filename)
{
	const auto code = read_collection(filename, ",", Converter<Integer>{});
	const auto img = fromProgram(code);
	std::cout << "Img:\n" << img.draw() << std::endl;
	std::cout << "Alignment: " << getAlignmentParameter(img) << std::endl;

	std::cout << "Path: " << PathBuilder::buildPath(img) << std::endl;
	std::cout << "Dust: " << dust(code) << std::endl;
}

int main()
{
	// TODO: Part 2 has to be partly solved by hand
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
