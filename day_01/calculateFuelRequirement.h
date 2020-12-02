#pragma once

#include <vector>

int calculateFuelRequirement(int mass);
int calculateFuelRequirementSum(const std::vector<int>& masses);

int calculateFuelRequirementRecursive(int mass);
int calculateFuelRequirementSumRecursive(const std::vector<int>& masses);