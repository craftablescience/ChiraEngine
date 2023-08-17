#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <typeindex>
#include <typeinfo>

namespace chira {

namespace detail {

//inline std::uint64_t typeIDSequenceController = 0;

template<typename T, typename F, std::size_t... Is>
constexpr void enumerateImpl(T&& t, F&& f, std::index_sequence<Is...>) {
    (f(std::integral_constant<std::size_t, Is>{}, std::get<Is>(t)), ...);
}

} // namespace detail

//template<typename T>
//inline const std::uint64_t typeID = detail::typeIDSequenceController++;

// todo(c++): switch to typeID variable template
template<typename T>
std::type_index typeHash() {
    return typeid(T);
}

template<typename T, typename F, std::size_t ElementCount = std::tuple_size_v<std::remove_reference_t<T>>>
constexpr void enumerate(T&& t, F&& f) {
    detail::enumerateImpl(t, std::forward<F>(f), std::make_index_sequence<ElementCount>{});
}

template<typename T, T... S, typename F>
constexpr void forSequence(std::integer_sequence<T, S...>, F&& f) {
    (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
}

} // namespace chira
