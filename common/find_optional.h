#pragma once

#ifdef USE_STD_OPTIONAL
#include <optional>
#elif defined(USE_EXPERIMENTAL_OPTIONAL)
#include <experimental/optional>
namespace std {

using experimental::nullopt_t;
using experimental::optional;

using experimental::make_optional;

constexpr auto nullopt = experimental::nullopt;

} // namespace std
#endif // defined(IAB_USE_STD_OPTIONAL)