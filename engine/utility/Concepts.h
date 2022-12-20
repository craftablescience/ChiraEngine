#pragma once

#include <concepts>
#include <type_traits>

namespace chira {

template<typename T>
concept CArithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept CNonArithmetic = !std::is_arithmetic_v<T>;

} // namespace chira
