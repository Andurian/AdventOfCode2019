#include <iostream>

#include "read_file.h"
#include "Intprogram.h"
#include "multicall.h"

void solve(const std::string& filename)
{
	const Code code = read_collection(filename, ",", Converter<Integer>{});

	{
		DequeInput in{ 1 };
		ConsoleOutput out{};

		std::cout << "ResultA: ";
		Intprogram{ in, out, code }.run();
		std::cout << std::endl;
	}

	{
		DequeInput in{ 2 };
		ConsoleOutput out{};

		std::cout << "ResultB: ";
		Intprogram{ in, out, code }.run();
		std::cout << std::endl;
	}
}


int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
