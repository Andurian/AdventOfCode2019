#define BOOST_TEST_MODULE day_06

#include <boost/test/included/unit_test.hpp>

#include "orbit_map.h"
#include "read_file.h"
#include "infix_ostream_iterator.h"


BOOST_AUTO_TEST_CASE(test_num_orbits_ordered)
{
  OrbitMap map{read_file("input_test_01.txt")};
  BOOST_TEST(map.numOrbits() == 42);
}

BOOST_AUTO_TEST_CASE(test_num_orbits_scrambled)
{
  OrbitMap map{read_file("input_test_02.txt")};
  BOOST_TEST(map.numOrbits() == 42);
}

BOOST_AUTO_TEST_CASE(test_find_path_from_com)
{
  OrbitMap map{read_file("input_test_03.txt")};

  std::vector<std::string> expectedToSan { "COM", "B", "C", "D", "I"};
  std::vector<std::string> expectedToYou { "COM", "B", "C", "D", "E", "J", "K"};

  auto outSan = map.pathTo("SAN");
  auto outYou = map.pathTo("YOU");

  BOOST_CHECK_EQUAL_COLLECTIONS(outSan.begin(), outSan.end(), expectedToSan.begin(), expectedToSan.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(outYou.begin(), outYou.end(), expectedToYou.begin(), expectedToYou.end());

}

BOOST_AUTO_TEST_CASE(test_find_path_between)
{
  OrbitMap map{read_file("input_test_03.txt")};

  std::vector<std::string> expected { "K", "J", "E", "D", "I"};

  auto out1 = map.pathBetween("YOU", "SAN");
  auto out2 = map.pathBetween("SAN", "YOU");

  BOOST_CHECK_EQUAL_COLLECTIONS(out1.begin(), out1.end(), expected.begin(), expected.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(out2.begin(), out2.end(), expected.rbegin(), expected.rend());
}

BOOST_AUTO_TEST_CASE(test_path_length_between)
{
  OrbitMap map{read_file("input_test_03.txt")};

  BOOST_TEST(map.pathLength("YOU", "SAN") == 4);
  BOOST_TEST(map.pathLength("SAN", "YOU") == 4);
}



