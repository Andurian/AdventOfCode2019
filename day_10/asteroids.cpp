#include "asteroids.h"

#include <cmath>
#include <iostream>

#include "read_file.h"


bool is_between(const Eigen::Vector2d & start, const Eigen::Vector2d & end, const Eigen::Vector2d & point)
{
	const Eigen::Vector2d vEnd = end - start;
	const Eigen::Vector2d vPoint = point - start;

	if (std::abs(vPoint(1) * vEnd(0) - vPoint(0) * vEnd(1)) > 0.001)
	{
		return false;
	}

	const auto dot = vEnd.dot(vPoint);
	return 0 < dot && dot < vEnd.squaredNorm();
}


double angle(const Eigen::Vector2d & a, const Eigen::Vector2d & b)
{
	const double dot = a.x() * b.x() + a.y() * b.y();
	const double det = a.x() * b.y() - a.y() * b.x();
	const double r = std::atan2(det, dot);

	return r >= 0 ? r : M_PI + M_PI + r;
}


std::vector<Asteroid> asteroids_from_file(const std::string & filename)
{
	return convert_image(read_collection(filename, "\n"));
}


std::vector<Asteroid> convert_image(const std::vector<std::string> & lines)
{
	std::vector<Asteroid> ret{};

	for (int row = 0; row < lines.size(); ++row)
	{
		for (int col = 0; col < lines[row].size(); ++col)
		{
			if (lines[row][col] == '#')
			{
				ret.emplace_back(col, row);
			}
		}
	}
	return ret;
}


void remove(const Asteroid & a, std::vector<Asteroid> & asteroids)
{
	asteroids.erase(std::find(asteroids.begin(), asteroids.end(), a));
}


bool can_see(const Asteroid & source, const Asteroid & target, const std::vector<Asteroid> & asteroids)
{
	for (const auto & a : asteroids)
	{
		if (a == source || a == target)
		{
			continue;
		}

		if (is_between(source, target, a))
		{
			return false;
		}
	}
	return true;
}


std::vector<Asteroid> all_can_see(const Asteroid & pos, const std::vector<Asteroid> & asteroids)
{
	std::vector<Asteroid> ret;
	for (const auto & a : asteroids)
	{
		if (a != pos && can_see(pos, a, asteroids))
		{
			ret.push_back(a);
		}
	}

	return ret;
}


int num_can_see(const Asteroid & pos, const std::vector<Asteroid> & asteroids)
{
	return static_cast<int>(all_can_see(pos, asteroids).size());
}


Asteroid best_asteroid(const std::vector<Asteroid> & asteroids)
{
	std::vector<int> num;
	for (const auto & a : asteroids)
	{
		num.push_back(num_can_see(a, asteroids));
	}

	const auto dist = std::max_element(num.begin(), num.end()) - num.begin();

	return *(asteroids.begin() + dist);
}


std::vector<Asteroid> sorted_clockwise(const Asteroid & ref, std::vector<Asteroid> asteroids)
{
	std::sort(asteroids.begin(), asteroids.end(), [&ref](const Asteroid & a, const Asteroid & b) {
		Eigen::Vector2d up = { 0, -1 };
		Eigen::Vector2d da = a - ref;
		Eigen::Vector2d db = b - ref;
		const auto angleA = angle(up, da);
		const auto angleB = angle(up, db);
		return angleA < angleB;
	});

	return asteroids;
}


std::vector<Asteroid> vaporization_order(const Eigen::Vector2d & pos, std::vector<Asteroid> asteroids)
{
	std::vector<Asteroid> ret;

	while (!asteroids.empty())
	{
		std::vector<Asteroid> round = sorted_clockwise(pos, all_can_see(pos, asteroids));
		ret.insert(ret.end(), round.begin(), round.end());

		for (const auto & vaporized : round)
		{
			remove(vaporized, asteroids);
		}
	}

	return ret;
}



