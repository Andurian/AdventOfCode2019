#include "calculateFuelRequirement.h"

#include <cmath>
#include <numeric>
#include <algorithm>


int calculateFuelRequirement(int mass)
{
	return std::max(0, static_cast<int>(std::floor(mass / 3.0)) - 2);
}


int calculateFuelRequirementSum(const std::vector<int>& masses)
{
	return std::accumulate(masses.begin(), masses.end(), 0, [](int sum, int x)
	{
		return sum + calculateFuelRequirement(x);
	});
}


int calculateFuelRequirementRecursive(int mass)
{
	int sum = calculateFuelRequirement(mass);
	int fuelForFuel = calculateFuelRequirement(sum);
	while(fuelForFuel > 0)
	{
		sum += fuelForFuel;
		fuelForFuel = calculateFuelRequirement(fuelForFuel);
	}

	return sum;
}


int calculateFuelRequirementSumRecursive(const std::vector<int>& masses)
{
	return std::accumulate(masses.begin(), masses.end(), 0, [](int sum, int x)
	{
		return sum + calculateFuelRequirementRecursive(x);
	});
}
