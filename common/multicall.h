#pragma once

#include <iostream>

namespace detail
{

	template<typename F, typename T>
	void call(int i, F func, T && arg)
	{
		std::cout << "Solutions Account " << i << ":" << std::endl;
		func(std::forward<T>(arg));
		std::cout << std::endl;
	}

	template<typename F, typename T>
	void multicall(int i, F func, T && arg)
	{
		call(i, func, std::forward<T>(arg));
	}

	template<typename F, typename T, typename... Tail>
	void multicall(int i, F func, T && arg, Tail &&... tail)
	{
		call(i, func, std::forward<T>(arg));
		multicall(i + 1, func, std::forward<Tail>(tail)...);
	}

} // namespace detail


template<typename F, typename... Args>
void multicall(F func, Args &&... args)
{
	detail::multicall(1, func, std::forward<Args>(args)...);
}