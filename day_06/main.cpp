#include <iostream>

#include "orbit_map.h"
#include "read_file.h"
#include "multicall.h"

void solve(const std::string& filename)
{
	OrbitMap map{read_file(filename)};

	std::cout << "Number of orbits: " << map.numOrbits() << std::endl;
	std::cout << "Number of hops: " << map.pathLength("YOU", "SAN");
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
}
