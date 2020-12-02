#pragma once

#include <set>
#include <map>

#include <Eigen/Core>

using Position = Eigen::Vector3i;
using Velocity = Eigen::Vector3i;


struct PositionComparator
{
	bool operator()(const Position & a, const Position & b) const
	{
		if (a.x() == b.x())
		{
			if (a.y() == b.y())
			{
				return a.z() < b.z();
			}
			return a.y() < b.y();
		}
		return a.x() < b.x();
	}
};


class Moon
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Moon(const Position & p);

	void applyGravity(const std::vector<Moon> & system);
	void step();

	int kineticEnergy() const;
	int potentialEnergy() const;
	int totalEnergy() const;

	const std::string toString() const;

	const auto & position() const { return m_pos; }

	const auto & velocity() const { return m_vel; }

	friend bool operator==(const Moon & a, const Moon & b);

private:
	Position m_pos;
	Velocity m_vel;
};


class System
{
public:
	System(const std::vector<Position> & initialPositions);

	void step();
	int totalEnergy() const;

	std::string toString() const;

	const auto & status() const { return m_moons; }

	auto steps() const { return m_steps; }

private:
	std::vector<Moon> m_moons;
	int m_steps;
};


bool operator==(const Moon & a, const Moon & b);


class CycleFinder
{
public:
	struct Cycle
	{
		using Values = std::tuple<int, int, int, int, int, int, int, int>;


		void update(const Values & v)
		{
			if (isDone())
			{
				return;
			}

			if(val.find(v) != val.end())
			{
				cycleLength = static_cast<int>(val.size());
			}
			else
			{
				val.insert(v);
			}
		}


		bool isDone() const { return cycleLength != -1; }

		std::set<Values> val = { };
		int cycleLength = -1;
	};


	CycleFinder(const std::vector<Position> & initialPositions);

	unsigned long long findCycleLength();

private:
	System m_system;

	Cycle m_X;
	Cycle m_Y;
	Cycle m_Z;
};
