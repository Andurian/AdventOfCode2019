#include <iostream>
#include "reaction.h"
#include "read_file.h"
#include "find_path_slow.h"
#include "multicall.h"

void solve(const std::string& filename)
{
	RuleSet rs = RuleSet::parse(read_file(filename));
	std::cout << "Minimum amount of ore: " << std::get<0>(find_path_all(rs, { {{"FUEL", 1}} })) << std::endl;

	std::cout << maxFuelFromOre(rs, 1000000000000) << std::endl;
}


int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
