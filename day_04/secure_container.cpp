#include "secure_container.h"

#include "read_file.h"


bool has_digits(const std::string & value, int numDigits)
{
	return value.length() == numDigits;
}


bool has_digits(int value, int numDigits)
{
	return has_digits(convert<std::string>(value), numDigits);
}


bool has_identical_neighbors(const std::string & value)
{
	if (value.empty())
	{
		return false;
	}

	auto it = value.begin();
	auto lastDigit = *it;
	for (++it; it != value.end(); ++it)
	{
		if (*it == lastDigit)
		{
			return true;
		}
		lastDigit = *it;
	}
	return false;
}


bool has_identical_neighbors(int value)
{
	return has_identical_neighbors(convert<std::string>(value));
}


bool is_not_decreasing(const std::string & value)
{
	if (value.empty())
	{
		return false;
	}

	auto it = value.begin();
	auto lastDigit = convert<int>(*it);
	for (++it; it != value.end(); ++it)
	{
		const auto currentDigit = convert<int>(*it);
		if (currentDigit < lastDigit)
		{
			return false;
		}
		lastDigit = currentDigit;
	}
	return true;
}


bool is_not_decreasing(int value)
{
	return is_not_decreasing(convert<std::string>(value));
}


bool is_password(int value)
{
	const auto s = convert<std::string>(value);
	return has_digits(s, 6) && is_not_decreasing(s) && has_identical_neighbors(s);
}


bool has_exactly_two_identical_neighbors(const std::string & value)
{
	if (value.length() < 2)
	{
		return false;
	}

	if (value.length() == 2)
	{
		return value[0] == value[1];
	}

	if (value[0] == value[1] && value[1] != value[2])
	{
		return true;
	}

	for (int i = 3; i < value.size(); ++i)
	{
		if (value[i - 3] != value[i - 2] && value[i - 2] == value[i - 1] && value[i - 1] != value[i])
		{
			return true;
		}
	}

	const auto l = value.length() - 1;
	return value[l - 2] != value[l - 1] && value[l - 1] == value[l];
}


bool has_exactly_two_identical_neighbors(int value)
{
	return has_exactly_two_identical_neighbors(convert<std::string>(value));
}


bool is_password_extended_rule(int value)
{
	const auto s = convert<std::string>(value);
	return has_digits(s, 6) && is_not_decreasing(s) && has_exactly_two_identical_neighbors(s);
}
