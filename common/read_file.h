#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <sstream>

#include "function_from_lambda.h"

template<typename T>
struct Converter
{
	template<typename U>
	T operator()(const U & u)
	{
		std::stringstream ss;
		ss << u;
		T t;
		ss >> t;
		return t;
	}
};

template<>
struct Converter<std::string>
{
	template<typename U>
	std::string operator()(const U & u)
	{
		std::stringstream ss;
		ss << u;
		return ss.str();
	}
};

template<typename T, typename U>
T convert(const U & u)
{
	return Converter<T>{}(u);
}

std::string read_file(const std::string & filename);
std::vector<std::string> split_string(const std::string & text, const std::string & delimiter);
std::vector<std::string> read_collection(const std::string & filename, const std::string & delimiter);

template<typename T>
std::vector<T> split_string(const std::string & text, Converter<T> convert)
{
	std::vector<T> ret;
	ret.reserve(text.size());

	std::transform(text.begin(), text.end(), std::back_inserter(ret), convert);
	return ret;
}

template<typename T>
std::vector<T> read_collection(const std::string & filename, const std::string & delimiter, Converter<T> converter)
{
	const auto strings = split_string(read_file(filename), delimiter);

	std::vector<T> ret;
	std::transform(strings.begin(), strings.end(), std::back_inserter(ret), converter);
	return ret;
}


template<typename T>
std::vector<T> read_collection(const std::string & filename, Converter<T> convert)
{
	return split_string(read_file(filename), convert);
}




