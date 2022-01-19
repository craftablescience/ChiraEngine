#pragma once

#include <cstddef>

namespace chira {
    using byte = std::uint8_t;

    /// Reverses the byte order of a type. Should really only be used with numbers.
    template<typename T>
    static inline T swapEndian(T t) {
        union {
            T t;
            byte u8[sizeof(T)];
        } source{}, dest{};
        source.t = t;
        for (size_t k = 0; k < sizeof(T); k++)
            dest.u8[k] = source.u8[sizeof(T) - k - 1];
        return dest.t;
    }
}
