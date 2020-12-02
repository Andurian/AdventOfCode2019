#include "read_file.h"

#include <fstream>
#include <sstream>

std::string read_file(const std::string & filename)
{
	std::stringstream ss;
	ss << std::ifstream{ filename }.rdbuf();
	return ss.str();
}

std::vector<std::string> split_string(const std::string & text, const std::string & delimiter)
{
	std::vector<std::string> ret;

	const auto length_delimiter = delimiter.size();

	size_t start_token = 0;
	size_t next_found = 0;
	while ((next_found = text.find(delimiter, start_token)) != std::string::npos)
	{
		const auto length_token = next_found - start_token;

		if (length_token > 0)
		{
			ret.push_back(text.substr(start_token, length_token));
		}
		start_token = next_found + length_delimiter;
	}

	if (start_token < text.size())
	{
		ret.push_back(text.substr(start_token));
	}

	return ret;
}
std::vector<std::string> read_collection(const std::string & filename, const std::string & delimiter)
{
	return split_string(read_file(filename), delimiter);
}
