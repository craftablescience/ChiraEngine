#pragma once

#include <string>
#include <string_view>

#include "Logger.h"

#ifndef _WIN32
    #include <csignal>
#endif

#ifdef __has_include
    // Broken up for portability
    #if __has_include(<source_location>)
        #define CHIRA_USE_SOURCE_LOCATION
        #include <source_location>
    #endif
#endif

#ifdef DEBUG
    #include <utility/Dialogs.h>
#endif

namespace chira {

inline void breakInDebugger() {
#ifdef _WIN32
    __debugbreak();
#else
    std::raise(SIGINT);
#endif
}

} // namespace chira

extern chira::LogChannel LOG_ASSERT;

// Leave outside the Chira namespace so it can be conditionally replaced with a macro
// (why did modern compilers not implement this??)
#ifdef CHIRA_USE_SOURCE_LOCATION
inline void runtime_assert(bool shouldAssert, std::string_view message, const std::source_location location = std::source_location::current()) {
    // Assertions fail when false
    if (shouldAssert)
        return;

    auto assertMsg = std::string{"Assertion failed!"} + "\n\n" +
            "In file: " + location.file_name() + '\n' +
            "In function: " + location.function_name() + '\n' +
            "At line: " + std::to_string(location.line()) + "\n\n" +
            message.data();
    LOG_ASSERT.error(assertMsg);

#ifdef DEBUG
    if (!chira::Dialogs::popupErrorChoice(assertMsg + "\n\nPress OK to continue, CANCEL to break in debugger.", false, "Assertion Failed"))
        chira::breakInDebugger();
#endif
}
#else
#define runtime_assert(shouldAssert, message) runtime_assert_internal(shouldAssert, message, __FILE__, __LINE__, __FUNCTION__)
inline void runtime_assert_internal(bool shouldAssert, std::string_view message, const char* file, int line, const char* function) {
    // Assertions fail when false
    if (shouldAssert)
        return;

    auto assertMsg = std::string{"Assertion failed!"} + "\n\n" +
                     "In file: " + file + '\n' +
                     "In function: " + function + '\n' +
                     "At line: " + std::to_string(line) + "\n\n" +
                     message.data();
    LOG_ASSERT.error(assertMsg);

#ifdef DEBUG
    if (!chira::Dialogs::popupErrorChoice(assertMsg + "\n\nPress OK to continue, CANCEL to break in debugger.", false, "Assertion Failed"))
        chira::breakInDebugger();
#endif
}
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

} // namespace chira
