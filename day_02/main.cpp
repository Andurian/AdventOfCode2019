#include <iostream>

#include "Intprogram.h"
#include "multicall.h"
#include "read_file.h"

Integer runModifiedProgram(const Code & code, int a, int b)
{
	Intprogram program{ code };
	program.write(1, a);
	program.write(2, b);
	program.run();
	return program.getCode()[0];
}

void solve(const std::string & filename)
{
	const auto code = read_collection(filename, ",", Converter<Integer>());

	std::cout << "Solution Part 1: " << runModifiedProgram(code, 12, 2) << std::endl;

	bool done = false;
	for (int a = 0; a < 100 && !done; ++a)
	{
		for (int b = 0; b < 100; ++b)
		{
			if (runModifiedProgram(code, a, b) == 19690720)
			{
				std::cout << "Solution Part 2: " << 100 * a + b << std::endl;
				done = true;
				break;
			}
		}
	}
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
