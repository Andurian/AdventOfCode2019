#include <iostream>

#include "read_file.h"
#include "multicall.h"
#include "wire_distance.h"

void solve(const std::string& filename)
{
	std::vector<std::string> wires = split_string(read_file(filename), "\n");

	std::cout << "Result A: " << closest_intersection(wires[0], wires[1]) << std::endl;
	std::cout << "Result B: " << closest_intersection_by_steps(wires[0], wires[1]) << std::endl;
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
