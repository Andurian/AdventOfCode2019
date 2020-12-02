#include "moons.h"

#include <infix_ostream_iterator.h>
#include <iomanip>
#include <sstream>

#include <boost/integer/common_factor.hpp>


template<typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}


Moon::Moon(const Position & p) : m_pos{ p }, m_vel{ 0, 0, 0 }
{
	// empty
}


void Moon::applyGravity(const std::vector<Moon> & system)
{
	for (const auto & moon : system)
	{
		if (moon == *this)
		{
			continue;
		}

		m_vel.x() += sgn(moon.position().x() - m_pos.x());
		m_vel.y() += sgn(moon.position().y() - m_pos.y());
		m_vel.z() += sgn(moon.position().z() - m_pos.z());
	}
}


void Moon::step()
{
	m_pos += m_vel;
}


const std::string Moon::toString() const
{
	const int w = 3;
	std::stringstream ss;
	ss << "pos=<x=" << std::setw(w) << m_pos.x() << ", y=" << std::setw(w) << m_pos.y() << ", z=" << std::setw(w) << m_pos.z()
	   << ">, vel=<x=" << std::setw(w) << m_vel.x() << ", y=" << std::setw(w) << m_vel.y() << ", z=" << std::setw(w) << m_vel.z()
	   << ">";
	return ss.str();
}


bool operator==(const Moon & a, const Moon & b)
{
	return a.m_pos == b.m_pos && a.m_vel == b.m_vel;
}


int Moon::kineticEnergy() const
{
	return m_pos.cwiseAbs().sum();
}


int Moon::potentialEnergy() const
{
	return m_vel.cwiseAbs().sum();
}


int Moon::totalEnergy() const
{
	return kineticEnergy() * potentialEnergy();
}


System::System(const std::vector<Position> & initialPositions) : m_steps{ 0 }
{
	m_moons.reserve(initialPositions.size());
	for (const auto & p : initialPositions)
	{
		m_moons.emplace_back(p);
	}
}


void System::step()
{
	for (auto & moon : m_moons)
	{
		moon.applyGravity(m_moons);
	}

	for (auto & moon : m_moons)
	{
		moon.step();
	}
	++m_steps;
}


std::string System::toString() const
{
	std::vector<std::string> str;
	for (const auto & moon : m_moons)
	{
		str.push_back(moon.toString());
	}

	std::stringstream ss;
	std::copy(str.begin(), str.end(), char_infix_ostream_iterator(ss, "\n"));
	return ss.str();
}


int System::totalEnergy() const
{
	int e = 0;
	for (const auto & m : m_moons)
	{
		e += m.totalEnergy();
	}
	return e;
}


CycleFinder::CycleFinder(const std::vector<Position> & initialPositions) : m_system{ initialPositions }
{
	// empty
}


unsigned long long CycleFinder::findCycleLength()
{
	while (!m_X.isDone() || !m_Y.isDone() || !m_Z.isDone())
	{
		const auto & stat = m_system.status();
		const auto & step = m_system.steps();

		m_X.update(std::make_tuple(
			stat[0].position().x(),
			stat[1].position().x(),
			stat[2].position().x(),
			stat[3].position().x(),
			stat[0].velocity().x(),
			stat[1].velocity().x(),
			stat[2].velocity().x(),
			stat[3].velocity().x()));

		m_Y.update(std::make_tuple(
			stat[0].position().y(),
			stat[1].position().y(),
			stat[2].position().y(),
			stat[3].position().y(),
			stat[0].velocity().y(),
			stat[1].velocity().y(),
			stat[2].velocity().y(),
			stat[3].velocity().y()));

		m_Z.update(std::make_tuple(
			stat[0].position().z(),
			stat[1].position().z(),
			stat[2].position().z(),
			stat[3].position().z(),
			stat[0].velocity().z(),
			stat[1].velocity().z(),
			stat[2].velocity().z(),
			stat[3].velocity().z()));

		m_system.step();
	}

	using namespace boost::integer;

	const auto x = static_cast<unsigned long long>(m_X.cycleLength);
	const auto y = static_cast<unsigned long long>(m_Y.cycleLength);
	const auto z = static_cast<unsigned long long>(m_Z.cycleLength);

	return lcm(x, y, z);
}
