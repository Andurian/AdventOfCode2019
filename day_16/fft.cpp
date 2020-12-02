#include "fft.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "read_file.h"

std::vector<int> getPositiveIndices(int round, int maxIndex)
{
	std::vector<int> ret;
	int k = 0;
	int j = 0;
	while (true)
	{
		int v = (round - 1) + k * round * 4 + j;
		if (v > maxIndex)
		{
			return ret;
		}
		ret.push_back(v);
		++j;
		if (j >= round)
		{
			++k;
			j = 0;
		}
	}
}
std::vector<int> getNegativeIndices(int round, int maxIndex)
{
	std::vector<int> ret;
	int k = 0;
	int j = 0;
	while (true)
	{
		int v = (3 * round - 1) + k * round * 4 + j;
		if (v > maxIndex)
		{
			return ret;
		}
		ret.push_back(v);
		++j;
		if (j >= round)
		{
			++k;
			j = 0;
		}
	}
}

int round(const std::vector<int> & v, int round)
{
	int ret = 0;

	const auto maxIdx = static_cast<int>(v.size() - 1);

	int k = 0;
	int j = 0;
	while (true)
	{
		int val = (3 * round - 1) + k * round * 4 + j;
		if (val > maxIdx)
		{
			break;
		}
		ret -= v[val];

		++j;
		if (j >= round)
		{
			++k;
			j = 0;
		}
	}

	k = 0;
	j = 0;
	while (true)
	{
		int val = (round - 1) + k * round * 4 + j;
		if (val > maxIdx)
		{
			return std::abs(ret) % 10;
		}
		ret += v[val];
		++j;
		if (j >= round)
		{
			++k;
			j = 0;
		}
	}
}

std::vector<int> toIntVector(const std::string str)
{
	std::vector<int> ret;
	std::transform(str.begin(), str.end(), std::back_inserter(ret), Converter<int>{});
	return ret;
}

std::string phase(const std::string & input)
{
	std::stringstream ss;

	const auto v = toIntVector(input);

	for (int r = 1; r <= v.size(); ++r)
	{
		ss << round(v, r);
	}

	return ss.str();
}

std::string fft(std::string input, int numPhases)
{
	for (int i = 0; i < numPhases; ++i)
	{
		input = phase(input);
		//std::cout << "Round " << i << " done" << std::endl;
	}
	return input;
}

std::string decode(std::string input, int numRepeats, int numPhases)
{
	std::stringstream ss;
	for (int i = 0; i < numRepeats; ++i)
	{
		ss << input;
	}

	auto sequence = toIntVector(ss.str());

	int offset = 0;
	for (int i = 0; i < 7; i++)
	{
		offset = offset * 10 + sequence[i];
	}

	//std::cout << offset << std::endl;

	std::vector<int> newSequence(sequence.size());

	int sum = 0;
	int phase = 0;

	while (phase < numPhases)
	{
		sum = 0;
		for (auto it = sequence.size() - 1; it >= sequence.size() / 2; it--)
		{
			sum += sequence[it];
			newSequence[it] = sum % 10;
		}
		sequence = newSequence;
		phase++;
	}

	ss.str("");


	for (int i = 0; i < 8; ++i)
	{
		ss << sequence[offset + i];
	}

	return ss.str();
}
