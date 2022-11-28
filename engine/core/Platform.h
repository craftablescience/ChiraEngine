#pragma once

#include <cstddef>
#include <cstdint>
#include <math/Types.h>

// 64-bit or 32-bit?
#if INTPTR_MAX == INT32_MAX
    #define CHIRA_32BIT
#elif INTPTR_MAX == INT64_MAX
    #define CHIRA_64BIT
#else
    #error "Environment is not 32-bit or 64-bit! What kind of machine are you even using?"
#endif

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    #define CHIRA_COMPILER_MSVC
#else
    #define CHIRA_COMPILER_INTEL
#endif
#if defined(__GNUC__) && !defined(__clang__)
    #define CHIRA_COMPILER_GNU
#endif
#if defined(__clang__)
    #define CHIRA_COMPILER_CLANG
#endif

#if defined(_WIN32)
    #define CHIRA_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
    #define CHIRA_PLATFORM_APPLE
#elif defined(__linux__)
    #define CHIRA_PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #define CHIRA_PLATFORM_XBSD
#elif defined(__sun)
    #define CHIRA_PLATFORM_SOLARIS
#else
    #error "Unknown platform! Please make a bug report on our GitHub"
#endif

namespace chira {

// This makes it a bit easier to use in strings and such
#if defined(CHIRA_32BIT)
    constexpr int ENVIRONMENT_TYPE = 32;
#elif defined(CHIRA_64BIT)
    constexpr int ENVIRONMENT_TYPE = 64;
#else
    #error "A CHIRA_XXBIT macro must be set!"
#endif

/// Changes the endianness of a type. Should only be used with numbers.
template<typename T> inline T swapEndian(T t) {
    union {
        T t;
        byte u8[sizeof(T)];
    } source{}, dest{};
    source.t = t;
    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    return dest.t;
}

} // namespace chira
