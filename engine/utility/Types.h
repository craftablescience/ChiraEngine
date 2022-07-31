#pragma once

#include <cstddef>
#include <typeinfo>

namespace chira {

template<typename T>
constexpr inline std::size_t getHashOfType() {
    return typeid(T).hash_code();
}

template <typename T, T... S, typename F>
constexpr inline void forSequence(std::integer_sequence<T, S...>, F&& f) {
    (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
}

} // namespace chira
