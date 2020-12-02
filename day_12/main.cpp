#include <iostream>

#include "moons.h"
#include "read_file.h"
#include "multicall.h"

void solve(const std::vector<Position>& initialPositions)
{
	System system{initialPositions};

	for(int i = 0; i < 1000; ++i)
	{
		system.step();
	}

	std::cout << "Total energy after 1000 steps: " << system.totalEnergy() << std::endl;
	std::cout << "Cycle Length: " << CycleFinder{ initialPositions }.findCycleLength() << std::endl;
}


int main()
{
	multicall(solve,
		std::vector<Position>{ { -1,   0, 2 }, { 2,  -10, -7}, {  4,  -8,  8 }, {  3,  5, -1 } }, // Test case 1
		std::vector<Position>{ { -8, -10, 0 }, { 5,    5, 10}, {  2,  -7,  3 }, {  9, -8, -3 } }, // Test case 2
		std::vector<Position>{ {  5,  -1, 5 }, { 0,  -14,  2}, { 16,   4,  0 }, { 18,  1, 16 } }, // Account 1
		std::vector<Position>{ {  4,   1, 1 }, { 11, -18, -1}, { -2, -10, -4 }, { -7, -2, 14 } }  // Account 2
	);
}
