#include <iostream>

#include "Graph.h"
#include "keylab.h"

#include "read_file.h"
#include "DistanceFunction.h"
#include "multicall.h"


void solve(const std::string& filename)
{
	auto img = Image::fromFile(filename);
	const auto start = img.findAll('@');
	const auto g = GraphBuilder::buildGraph(img, start);
	std::cout << g.draw() << std::endl;

	const auto dist = KeyDistanceFunction{ g, start };

	std::cout << "x" << std::endl;

	std::cout << "P: " << anotherPathFindingAttempt(g, start);
}

int main()
{
	multicall(solve, "input_account_02_open.txt", "input_account_02_closed.txt");
	return 0;
}
