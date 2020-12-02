#include <iostream>

#include "asteroids.h"
#include "read_file.h"
#include "multicall.h"

void solve(const std::string& filename)
{
	std::vector<Asteroid> asteroids = convert_image(read_collection(filename, "\n"));

	const auto & pos = best_asteroid(asteroids);

	asteroids.erase(std::find(asteroids.begin(), asteroids.end(), pos));

	std::cout << "Result A: " << num_can_see(pos, asteroids) << std::endl;

	const auto vap = vaporization_order(pos, asteroids);
	std::cout << "Result B: " << vap[199].x() * 100 + vap[199].y() << std::endl;
}

int main()
{
	multicall(solve, "input_account_01.txt", "input_account_02.txt");
	return 0;
}
