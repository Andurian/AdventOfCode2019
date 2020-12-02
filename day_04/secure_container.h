#pragma once

#include <string>

bool has_digits(const std::string & value, int numDigits);
bool has_digits(int value, int numDigits);

bool has_identical_neighbors(const std::string & value);
bool has_identical_neighbors(int value);

bool is_not_decreasing(const std::string & value);
bool is_not_decreasing(int value);

bool is_password(int value);

bool has_exactly_two_identical_neighbors(const std::string & value);
bool has_exactly_two_identical_neighbors(int value);

bool is_password_extended_rule(int value);