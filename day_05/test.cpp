#define BOOST_TEST_MODULE day_05

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <iostream>

#include "Intprogram.h"


BOOST_AUTO_TEST_CASE(test_intprogram_io)
{
	const std::vector<Integer> input{ 3, 0, 4, 0, 99 };

	for (int i = 11; i < 44; ++i)
	{
		DequeInput in{ i };
		DequeOutput out;

		Intprogram program{ in, out, input };
		program.run();

		BOOST_TEST(out.getValues()[0] == i);
	}
}


BOOST_AUTO_TEST_CASE(test_intprogram_io2)
{
	const std::vector<Integer> input{ 3, 0, 4, 0, 99 };

	for (int i = 11; i < 44; ++i)
	{
		DequeInput in{ i };
		ConsoleOutput out;

		Intprogram program{ in, out, input };
		program.run();
	}
}


BOOST_AUTO_TEST_CASE(test_intprogram_mul)
{
	const std::vector<Integer> input{ 1002, 4, 3, 4, 33 };
	const std::vector<Integer> expected{ 1002, 4, 3, 4, 99 };

	Intprogram program{ input };
	program.step();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_negative_number)
{
	const std::vector<Integer> input{ 1101, 100, -1, 4, 0 };
	const std::vector<Integer> expected{ 1101, 100, -1, 4, 99 };

	Intprogram program{ input };
	program.step();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_comparison_01)
{
	const std::vector<Integer> code = { 3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8 };

	{
		DequeInput in{ -100 };
		DequeOutput out;

		Intprogram{ in, out, code }.run();

		BOOST_TEST(out.getValues()[0] == 0);
	}

	{
		DequeInput in{ 8 };
		DequeOutput out;

		Intprogram{ in, out, code }.run();

		BOOST_TEST(out.getValues()[0] == 1);
	}
}


BOOST_AUTO_TEST_CASE(test_intprogram_comparison_02)
{
	const std::vector<Integer> code = {
		3, 21, 1008, 21, 8, 20, 1005, 20, 22, 107, 8, 21, 20, 1006, 20, 31,
		1106, 0, 36, 98, 0, 0, 1002, 21, 125, 20, 4, 20, 1105, 1, 46, 104,
		999, 1105, 1, 46, 1101, 1000, 1, 20, 4, 20, 1105, 1, 46, 98, 99
	};

	for(int i = -10; i < 20; ++i)
	{
		DequeInput in{ i };
		DequeOutput out;

		Intprogram{ in, out, code }.run();

		auto result = out.getValues()[0];

		if (i < 8)
		{
			BOOST_TEST(result == 999);
		}
		else if(i == 8)
		{
			BOOST_TEST(result == 1000);
		}
		else
		{
			BOOST_TEST(result == 1001);
		}
	}
}
