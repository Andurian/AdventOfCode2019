#define BOOST_TEST_MODULE day_09

#include <boost/test/included/unit_test.hpp>

#include <vector>
#include "Intprogram.h"
#include "read_file.h"


BOOST_AUTO_TEST_CASE(test_intprogram_quine)
{
	const Code code{ 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 };

	DequeInput in{};
	DequeOutput out{};
	Intprogram{ in, out, code }.run();

	BOOST_CHECK_EQUAL_COLLECTIONS(out.getValues().begin(), out.getValues().end(), code.begin(), code.end());
}

BOOST_AUTO_TEST_CASE(test_intprogram_digits_01)
{
	const Code code{ 1102,34915192,34915192,7,4,7,99,0 };

	DequeInput in{};
	DequeOutput out{};
	Intprogram{ in, out, code }.run();

	auto res = convert<std::string>(out.getValues()[0]);

	BOOST_TEST(res.length() == 16);
}

BOOST_AUTO_TEST_CASE(test_intprogram_digits_02)
{
	const Code code{ 104,1125899906842624,99 };

	DequeInput in{};
	DequeOutput out{};
	Intprogram{ in, out, code }.run();

	BOOST_TEST(out.getValues()[0] == 1125899906842624);
}
