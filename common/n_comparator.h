#pragma once

#include <cstdlib>

template<typename T, std::size_t N>
struct NComparator
{
	bool operator()(const T & a, const T & b) const
	{
		for(std::size_t i = 0; i < N; ++i)
		{
			if(a(i) == b(i))
			{
				continue;
			}
			return a(i) < b(i);
		}
		return false;
	}
};
