#define BOOST_TEST_MODULE day_14

#include <boost/test/included/unit_test.hpp>

#include <vector>

#include "reaction.h"
#include "read_file.h"
#include "find_path_slow.h"


BOOST_AUTO_TEST_CASE(test_parse_rule)
{
	std::string formula = "10 ORE => 10 A";
	BOOST_TEST(convert<std::string>(Rule::parse(formula)) == formula);

	formula = "67 KGVR, 1 ZLJR, 4 TBPB, 19 KPJZM, 8 QSWQ, 12 DTQTB, 15 QRSD, 4 FPKWZ => 1 FUEL";
	BOOST_TEST(convert<std::string>(Rule::parse(formula)) == formula);
}


BOOST_AUTO_TEST_CASE(test_contingent)
{
	Contingent c{ { { "A", 1 }, { "B", 2 } } };

	BOOST_TEST(c.amountOf("A") == 1);
	BOOST_TEST(c.has("B", 1));
	BOOST_TEST(c.has("B", 2));
	BOOST_TEST(!c.has("B", 3));
	c.remove("A", 1);
	BOOST_TEST(!c.has("A", 1));
	try
	{
		c.remove("B", 5);
		BOOST_TEST(false);
	}
	catch (std::runtime_error &)
	{
		BOOST_TEST(true);
	}
	BOOST_TEST(c.amountOf("B") == 2);
}


BOOST_AUTO_TEST_CASE(test_find_path_slow)
{
	const std::string input =
		R"VV(9 ORE => 2 A
8 ORE => 3 B
7 ORE => 5 C
3 A, 4 B => 1 AB
5 B, 7 C => 1 BC
4 C, 1 A => 1 CA
2 AB, 3 BC, 4 CA => 1 FUEL)VV";

	const auto ruleSet = RuleSet::parse(input);
	Contingent target{ { { "FUEL", 1 } } };

	const auto res = find_path_slow(ruleSet, target);

	BOOST_TEST(res.value().amount == 165);
}
