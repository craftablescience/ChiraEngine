#pragma once

#include <string_view>

#include "Platform.h"

#ifndef CHIRA_PLATFORM_WINDOWS
    #include <csignal>
#endif

// Have to do this crap pile because std::source_location can be unimplemented even if the header exists
#ifdef __has_include
    // Broken up for portability
    #if __has_include(<source_location>)
        #include <source_location>
        // This macro is defined in <source_location> if its implemented
        #ifdef __cpp_lib_source_location
            #define CHIRA_USE_SOURCE_LOCATION
        #endif
    #endif
#endif

// Leave outside the Chira namespace so it can be conditionally replaced with a macro
// (why did modern compilers not implement this??)
#ifdef CHIRA_USE_SOURCE_LOCATION
void runtime_assert(bool shouldAssert, std::string_view message, const std::source_location& location = std::source_location::current());
#else
#define runtime_assert(shouldAssert, message) runtime_assert_internal(shouldAssert, message, __FILE__, __LINE__, __FUNCTION__)
void runtime_assert_internal(bool shouldAssert, std::string_view message, const char* file, int line, const char* function);
#endif

namespace chira {

template<typename T>
inline T assert_cast(auto obj) {
#ifdef DEBUG
    if (!obj)
        return nullptr;
    auto cast = dynamic_cast<T>(obj);
    runtime_assert(cast, "Object could not be cast");
    return cast;
#else
    return static_cast<T>(obj);
#endif
}

inline void breakInDebugger() {
#ifdef CHIRA_PLATFORM_WINDOWS
    __debugbreak();
#else
    std::raise(SIGINT);
#endif
}

} // namespace chira
