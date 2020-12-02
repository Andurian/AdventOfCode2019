#include <iostream>

#include "read_file.h"
#include "calculateFuelRequirement.h"
#include "multicall.h"

void solve(const std::string& filename)
{
	const auto masses = read_collection(filename, "\n", Converter<int>());
	const int requiredFuel = calculateFuelRequirementSum(masses);

	std::cout << "Required Fuel: " << requiredFuel << std::endl;

	const int requiredFuelTotal = calculateFuelRequirementSumRecursive(masses);

	std::cout << "Required Total Fuel: " << requiredFuelTotal << std::endl;
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}