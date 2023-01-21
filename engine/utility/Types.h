#pragma once

#include <cstddef>
#include <typeindex>

namespace chira {

// todo(c++): make this consteval when possible!
template<typename T>
std::type_index typeHash() {
    return typeid(T);
}

template<typename T, T... S, typename F>
constexpr void forSequence(std::integer_sequence<T, S...>, F&& f) {
    (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
}

} // namespace chira
