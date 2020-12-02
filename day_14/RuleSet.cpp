#include "RuleSet.h"

#include "read_file.h"


RuleSet::RuleSet(std::vector<Rule> rules)
	: m_rules{ std::move(rules) }
{
	// empty
}


bool RuleSet::applySingleStep(Contingent & c) const
{
	for (const auto & r : m_rules)
	{
		if (r.canApplyInverse(c))
		{
			r.applyInverse(c);

			return true;
		}
	}
	return false;
}


void RuleSet::applyRepeated(Contingent & c) const
{
	//std::cout << c << std::endl;
	bool changed = applySingleStep(c);
	while (changed)
	{
		//std::cout << c << std::endl;
		changed = applySingleStep(c);
	}
}


std::vector<Rule::Material> RuleSet::necessaryIncreases(const Contingent & c) const
{
	std::vector<Rule::Material> ret;
	for (const auto & r : m_rules)
	{
		const auto existingAmount = c.amountOf(r.m_output.name);
		if (existingAmount > 0)
		{
			ret.push_back(Rule::Material{ r.m_output.name, r.m_output.quantity - existingAmount });
		}
	}
	return ret;
}


RuleSet RuleSet::parse(const std::string & formulas)
{
	const auto lines = split_string(formulas, "\n");
	std::vector<Rule> rules;
	for (const auto & l : lines)
	{
		rules.push_back(Rule::parse(l));
	}

	return rules;
}


std::ostream & operator<<(std::ostream & os, const RuleSet & rs)
{
	os << rs.m_rules.front();

	for (auto it = rs.m_rules.begin() + 1; it != rs.m_rules.end(); ++it)
	{
		os << "\n" << *it;
	}

	return os;
}
