#include "find_path_slow.h"

#include <algorithm>


std::ostream & operator<<(std::ostream & os, const ModificationSet & m)
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


std::optional<Result> find_path_slow(const RuleSet & rules, Contingent c, ModificationSet appliedModifications)
{
	std::cout << "Initial: " << c << std::endl;
	rules.applyRepeated(c);

	if (c.size() == 1 && c.has("ORE", 1))
	{
		std::cout << "FINISHED: " << c << " " << appliedModifications << std::endl;
		return Result{ c.amountOf("ORE"), appliedModifications };
	}

	std::cout << "Stuck at: " << c << std::endl;
	auto increases = rules.necessaryIncreases(c);
	std::cout << "Possibilities: " << increases << std::endl;
	std::vector<Result> minPath;

	for (const auto & i : increases)
	{
		if (appliedModifications.find(i) != appliedModifications.end())
		{
			continue;
		}

		Contingent c_modified = c;
		c_modified.add(i.name, i.quantity);
		appliedModifications.insert(i);

		std::cout << "Current Modifications: " << appliedModifications << std::endl;
		std::cout << "Trying new Path: " << c_modified << std::endl;

		const auto r = find_path_slow(rules, c_modified, appliedModifications);
		if (r)
		{
			minPath.push_back(*r);
		}
	}

	if (minPath.empty())
	{
		return std::nullopt;
	}

	return *std::min_element(
		minPath.begin(), minPath.end(), [](const Result & a, const Result & b) { return a.amount < b.amount; });
}


std::tuple<Integer, Contingent> find_path_all(const RuleSet & rules, Contingent c, Contingent surplus)
{
	rules.applyRepeated(c);

	if (c.size() == 1 && c.has("ORE", 1))
	{
		rules.applyRepeated(surplus);
		auto total = c.amountOf("ORE");
		const auto extraOre = surplus.amountOf("ORE");
		if (extraOre > 0)
		{
			total -= extraOre;
			surplus.remove("ORE", extraOre);
		}
		return std::make_tuple(total, surplus);
	}

	auto increases = rules.necessaryIncreases(c);

	Contingent c_modified = c;
	for (const auto & i : increases)
	{
		c_modified.add(i.name, i.quantity);
		surplus.add(i.name, i.quantity);
	}

	return find_path_all(rules, c_modified, surplus);
}


Integer maxFuelFromOre(const RuleSet & rules, Integer ore)
{
	const auto oreForOne = std::get<0>(find_path_all(rules, { { { "FUEL", 1 } } }));

	Integer count = 0;
	Contingent materials{ {} };

	while (true)
	{
		const auto estimate = (ore - materials.amountOf("ORE") + oreForOne - 1) / oreForOne;

		auto temp = materials;
		if (temp.has("ORE", 1))
		{
			temp.remove("ORE", temp.amountOf("ORE"));
		}
		temp.add("FUEL", estimate);

		if (std::get<0>(find_path_all(rules, temp)) > ore - materials.amountOf("ORE"))
		{
			std::cout << "DONE 1" << std::endl;
			break;
		}

		materials.add("FUEL", estimate);
		rules.applyRepeated(materials);
		count += estimate;

		//std::cout << "ORE: " << ore - materials.amountOf("ORE") << " COUNT: " << count << std::endl;
	}

	while (true)
	{
		auto temp = materials;
		if (temp.has("ORE", 1))
		{
			temp.remove("ORE", temp.amountOf("ORE"));
		}
		temp.add("FUEL", 1);

		if(std::get<0>(find_path_all(rules,temp)) > ore - materials.amountOf("ORE"))
		{
			break;
		}

		materials.add("FUEL", 1);
		rules.applyRepeated(materials);
		count += 1;
		//std::cout << "ORE: " << ore - materials.amountOf("ORE") << " COUNT: " << count << std::endl;
	}

	return count; //TODO: Can apparently be off by +1

}
