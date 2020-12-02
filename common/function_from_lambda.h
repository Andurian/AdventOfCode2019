#pragma once

#include <functional>

namespace detail
{
	template<typename T>
	struct function_traits
	{
		using type = void;
	};

	template<typename Ret, typename Class, typename... Args>
	struct function_traits<Ret(Class::*)(Args...) const>
	{
		using type = std::function<Ret(Args...)>;
	};

	template<typename F>
	typename function_traits<decltype(&F::operator())>::type function_from_lambda(const F & func)
	{ 
		return func;
	}
}