#pragma once

#include <set>
#include <iostream>
#include "find_optional.h"

#include "Rule.h"
#include "RuleSet.h"


using Modification = Rule::Material;


struct ModificationComparator
{
	bool operator()(const Modification & a, const Modification & b) const
	{
		if (a.name == b.name)
		{
			return a.quantity < b.quantity;
		}

		return a.name < b.name;
	}
};


using ModificationSet = std::set<Modification, ModificationComparator>;
std::ostream & operator<<(std::ostream & os, const ModificationSet & m);


struct Result
{
	Integer amount;
	ModificationSet appliedModifications;
};


std::optional<Result> find_path_slow(const RuleSet & rules, Contingent c, ModificationSet appliedModifications = { });
std::tuple<Integer, Contingent> find_path_all(const RuleSet & rules, Contingent c, Contingent surplus = Contingent{{}});
Integer maxFuelFromOre(const RuleSet & rules, Integer ore);