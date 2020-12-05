#include <functional>
#include <iostream>

#include "multicall.h"
#include "secure_container.h"

int count(const std::function<bool(int)> & filter, int start, int end)
{
	int num = 0;
	for (int i = start; i <= end; ++i)
	{
		if (filter(i))
		{
			++num;
		}
	}
	return num;
}

void solve(std::pair<int, int> range)
{
	std::cout << "Possible passwords: " << count(is_password, range.first, range.second) << std::endl;
	std::cout << "Possible passwords (extended): " << count(is_password_extended_rule, range.first, range.second);
}


int main()
{
	multicall(solve, std::make_pair(382345, 843167), std::make_pair(367479, 893698));
	return 0;
}
