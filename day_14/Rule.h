#pragma once

#include <string>
#include <vector>

#include "Contingent.h"

class Rule
{
public:
	struct Material
	{
		std::string name;
		Integer quantity;

		static Material parse(const std::string & formula);
	};

	Rule(std::vector<Material> input, Material output);

	void applyInverse(Contingent & c) const;
	bool canApplyInverse(const Contingent & c) const;

	bool canApply(const Contingent & c) const;
	void apply(Contingent & c) const;

	static Rule parse(const std::string & formula);

	friend class RuleSet;
	friend std::ostream & operator<<(std::ostream & os, const Rule & r);

private:
	std::vector<Material> m_input;
	Material m_output;
};


std::ostream & operator<<(std::ostream & os, const Rule & r);
std::ostream & operator<<(std::ostream & os, const Rule::Material & mat);
std::ostream & operator<<(std::ostream & os, const std::vector<Rule::Material> & m);

