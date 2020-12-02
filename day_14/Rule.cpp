#include "Rule.h"

#include <cassert>

#include "read_file.h"


Rule::Material Rule::Material::parse(const std::string & formula)
{
	const auto temp = split_string(formula, " ");

	assert(temp.size() == 2);

	return Material{ temp[1], convert<int>(temp[0]) };
}


Rule::Rule(std::vector<Material> input, Material output)
	: m_input{ std::move(input) }
	, m_output{ std::move(output) }
{
	// empty
}


void Rule::applyInverse(Contingent & c) const
{
	if (!canApplyInverse(c))
	{
		throw std::runtime_error("Not enough material");
	}

	const auto times = c.amountOf(m_output.name) / m_output.quantity;

	c.remove(m_output.name, times * m_output.quantity);

	for (const auto & m : m_input)
	{
		c.add(m.name, times * m.quantity);
	}
}


bool Rule::canApplyInverse(const Contingent & c) const
{
	return c.has(m_output.name, m_output.quantity);
}


Rule Rule::parse(const std::string & formula)
{
	const auto halfes = split_string(formula, " => ");

	assert(halfes.size() == 2);

	const auto & left = halfes[0];
	const auto & right = halfes[1];

	const auto inputParts = split_string(left, ", ");
	std::vector<Material> input;
	input.reserve(inputParts.size());

	for (const auto & part : inputParts)
	{
		input.push_back(Material::parse(part));
	}

	const auto output = Material::parse(right);

	return Rule(input, output);
}


bool Rule::canApply(const Contingent & c) const
{
	for (const auto & m : m_input)
	{
		if (!c.has(m.name, m.quantity))
		{
			return false;
		}
	}
	return true;
}


void Rule::apply(Contingent & c) const
{
	if (!canApply(c))
	{
		throw std::runtime_error("");
	}

	for (const auto & m : m_input)
	{
		c.remove(m.name, m.quantity);
	}

	c.add(m_output.name, m_output.quantity);
}


std::ostream & operator<<(std::ostream & os, const Rule::Material & mat)
{
	return os << mat.quantity << " " << mat.name;
}


std::ostream & operator<<(std::ostream & os, const Rule & r)
{
	os << r.m_input[0];

	for (auto it = r.m_input.begin() + 1; it != r.m_input.end(); ++it)
	{
		os << ", " << *it;
	}

	return os << " => " << r.m_output;
}


std::ostream & operator<<(std::ostream & os, const std::vector<Rule::Material> & m)
{
	if (m.empty())
	{
		return os << "[ ]";
	}

	auto it = m.begin();
	os << "[" << it->name << " " << it->quantity;
	for (++it; it != m.end(); ++it)
	{
		os << ", " << it->name << " " << it->quantity;
	}
	return os << "]";
}
