#pragma once

#include <csignal>
#include <string>
#include <utility/logger.h>
#ifdef DEBUG
#include <utility/dialogs.h>
#endif

namespace chira {
    inline void breakInDebugger() {
#ifdef _WIN32
        __debugbreak();
#else
        std::raise(SIGINT);
#endif
    }

    inline void _assert_internal(bool shouldAssert, const std::string& message, bool isFatal, const char* FILE, int LINE) {
        // Assertions fail when false
        if (shouldAssert)
            return;
        // Not translated because translation manager uses asserts
        // Also because this needs to be 100% stable by itself
        std::string assertPrefix = std::string{isFatal ? "Fatal assertion" : "Assertion"} + " in file " + FILE + " at line " + std::to_string(LINE) + ':';
#pragma push_macro("ERROR")
#undef ERROR
        Logger::log(LogType::ERROR, "Assert", assertPrefix + ' ' + message);
#pragma pop_macro("ERROR")
#ifdef DEBUG
        if (!dialogPopupErrorChoice(assertPrefix + "\n\n" + message + "\n\nPress OK to continue, CANCEL to break in debugger.", false, "Assertion Failed"))
            breakInDebugger();
#endif
        if (isFatal)
            abort();
    }
}

// should use std::source_location if ever moving to C++20
#define chira_assert(shouldAssert, message) chira::_assert_internal(shouldAssert, message, false, __FILE__, __LINE__)
#define chira_assert_fatal(shouldAssert, message) chira::_assert_internal(shouldAssert, message, true, __FILE__, __LINE__)

#ifdef DEBUG
// Outside the chira namespace so it can be replaced with static_cast in release
template<typename T, typename U>
inline T assert_cast(U obj) {
    auto cast = dynamic_cast<T>(obj);
    chira_assert(cast, "Object could not be cast");
    return cast;
}
#else
#define assert_cast static_cast
#endif
