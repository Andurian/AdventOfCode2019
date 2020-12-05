#include <iostream>

#include "arcade.h"
#include "multicall.h"


void solve(const std::string& filename)
{
	{
		Arcade a{-1};
		a.run(filename, false);
		std::cout << "Number of block tiles: " << a.getTileCount(Arcade::Block) << std::endl;
	}

	{
		Arcade a{-1};
		a.run(filename, true);
		std::cout << "Final Score: " << a.getScore() << std::endl;
	}
}

void watchGame(const std::string& filename)
{
	Arcade{10}.run(filename, true);
}


int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");

	watchGame("input_account_01.txt");

	return 0;
}
