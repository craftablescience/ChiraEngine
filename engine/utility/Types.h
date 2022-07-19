#pragma once

#include <cstddef>
#include <typeinfo>

namespace chira {

template<typename T> inline std::size_t getHashOfType() {
    return typeid(T).hash_code();
}

}
