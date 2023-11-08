#pragma once

#include <cstddef>
#include <cstdint>
#include <typeindex>
#include <typeinfo>

namespace chira {

template<typename T>
inline std::type_index typeIndex() {
    return {typeid(T)};
}

template<typename T>
inline std::size_t typeHash() {
    return typeid(T).hash_code();
}

} // namespace chira
