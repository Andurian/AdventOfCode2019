#include <iostream>

#include "read_file.h"
#include "amplifiers.h"
#include "multicall.h"


void solve(const std::string& filename)
{
	const Code code = read_collection(filename, ",", Converter<Integer>{});

	std::cout << "Result A: " << max_amplifier_signal(5, code) << std::endl;
	std::cout << "Result B: " << max_amplifier_signal_loop({ 5, 6, 7, 8, 9 }, code) << std::endl;
}


int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
}
