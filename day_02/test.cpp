#define BOOST_TEST_MODULE day_02

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include "Intprogram.h"

BOOST_AUTO_TEST_CASE(test_intprogram_constructible)
{
	Intprogram{ std::vector<Integer>{ 1, 2, 3, 4 } };
}


BOOST_AUTO_TEST_CASE(test_intprogram_add)
{
	const std::vector<Integer> input{ 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50 };
	const std::vector<Integer> expected{ 1, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 };

	Intprogram program{ input };
	program.step();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_mul)
{
	const std::vector<Integer> input{ 1, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 };
	const std::vector<Integer> expected{ 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 };

	Intprogram program{ input };
	program.resetInstructionPointer(4);
	program.step();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_run_1)
{
	const std::vector<Integer> input{ 1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50 };
	const std::vector<Integer> expected{ 3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50 };

	Intprogram program{ input };
	program.run();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_run_2)
{
	const std::vector<Integer> input{ 1, 0, 0, 0, 99 };
	const std::vector<Integer> expected{ 2, 0, 0, 0, 99 };

	Intprogram program{ input };
	program.run();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_run_3)
{
	const std::vector<Integer> input{ 2, 3, 0, 3, 99 };
	const std::vector<Integer> expected{ 2, 3, 0, 6, 99 };

	Intprogram program{ input };
	program.run();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_run_4)
{
	const std::vector<Integer> input{ 2, 4, 4, 5, 99, 0 };
	const std::vector<Integer> expected{ 2, 4, 4, 5, 99, 9801 };

	Intprogram program{ input };
	program.run();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_run_5)
{
	const std::vector<Integer> input{ 1, 1, 1, 4, 99, 5, 6, 0, 99 };
	const std::vector<Integer> expected{ 30, 1, 1, 4, 2, 5, 6, 0, 99 };

	Intprogram program{ input };
	program.run();

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}


BOOST_AUTO_TEST_CASE(test_intprogram_amend)
{
	const std::vector<Integer> input{ 1, 1, 1, 4, 99, 5, 6, 0, 99 };
	const std::vector<Integer> expected{ 1, 12, 2, 4, 99, 5, 6, 0, 99 };

	Intprogram program{ input };
	program.write(1, 12);
	program.write(2, 2);

	const auto output = program.getCode();

	BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}
