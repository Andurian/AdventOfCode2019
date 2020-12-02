#include "Contingent.h"


Contingent::Contingent(std::unordered_map<std::string, Integer> initialValues)
	: m_materials{ std::move(initialValues) }
{
	// empty
}


bool Contingent::has(const std::string & name, Integer quantity) const
{
	return amountOf(name) >= quantity;
}


void Contingent::remove(const std::string & name, Integer quantity)
{
	if (!has(name, quantity))
	{
		throw std::runtime_error("Not enough material");
	}

	const auto it = m_materials.find(name);
	if (it->second == quantity)
	{
		m_materials.erase(it);
	}
	else
	{
		it->second -= quantity;
	}
}


void Contingent::add(const std::string & name, Integer quantity)
{
	const auto [it, successful] = m_materials.insert(std::make_pair(name, 0));
	it->second += quantity;
}


Integer Contingent::amountOf(const std::string & name) const
{
	const auto it = m_materials.find(name);
	if (it == m_materials.end())
	{
		return 0;
	}

	return it->second;
}


std::ostream & operator<<(std::ostream & os, const Contingent & c)
{
	auto it = c.m_materials.begin();
	os << "[" << it->second << " " << it->first;
	for (++it; it != c.m_materials.end(); ++it)
	{
		os << ", " << it->second << " " << it->first;
	}
	return os << "]";
}