#include <iostream>
#include <sstream>
#include "read_file.h"
#include "Intprogram.h"
#include "multicall.h"

void runWithInput(const std::string& filename, int input)
{
	Code code = read_collection(filename, ",", Converter<Integer>{});
	DequeInput in{ input };
	ConsoleOutput out;
	Intprogram program{in, out, code };

	std::cout << "Output: ";
	program.run();
	std::cout << std::endl;
}

void solve(const std::string& filename)
{
	runWithInput(filename, 1);
	runWithInput(filename, 5);
}


int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}