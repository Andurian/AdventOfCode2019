#include <iostream>

#include "read_file.h"

#include "Graph.h"
#include "Image.h"
#include "donut.h"
#include "multicall.h"

void solve(const std::string& filename)
{
	auto img = Image::fromFile(filename);

	std::cout << "Distance: " << findPath(GraphBuilder::buildGraph(img)) << std::endl;
	std::cout << "Recursive Distance: " << findRecursablePath(GraphBuilder::buildRecursableGraph(img)) << std::endl;
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
