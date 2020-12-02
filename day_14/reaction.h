#pragma once

#include <map>

#include "Rule.h"
#include "RuleSet.h"

class MaterialFinder
{
public:
	struct CacheEntry { int minAmount; int ore; };

	MaterialFinder(RuleSet rules);

	int findMaterialFor(const std::string & mat, int amount);

private:
	RuleSet m_rules;
	Contingent m_tooMuchCreated;
	std::map<std::string, CacheEntry> m_cache;
};

