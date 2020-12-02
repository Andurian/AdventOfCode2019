#pragma once

#include <vector>

#include "Contingent.h"
#include "Rule.h"


class RuleSet
{
public:
	RuleSet(std::vector<Rule> rules);

	bool applySingleStep(Contingent & c) const;
	void applyRepeated(Contingent & c) const;

	std::vector<Rule::Material> necessaryIncreases(const Contingent & c) const;

	static RuleSet parse(const std::string & formulas);

	friend std::ostream & operator<<(std::ostream & os, const RuleSet & rs);

private:
	std::vector<Rule> m_rules;
};


std::ostream & operator<<(std::ostream & os, const RuleSet & rs);