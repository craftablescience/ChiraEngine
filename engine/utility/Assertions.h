#pragma once

#ifndef _WIN32
    #include <csignal>
#endif
#include <string>
#include <source_location>

#include <utility/Logger.h>
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

inline void runtime_assert(bool shouldAssert, const std::string& message, bool isFatal = false, const std::source_location location = std::source_location::current()) {
    // Assertions fail when false
    if (shouldAssert)
        return;

    auto assertMsg = std::string{isFatal ? "Fatal assertion!" : "Assertion!"} + "\n\n" +
            "In file: " + location.file_name() + '\n' +
            "In function: " + location.function_name() + '\n' +
            "At line: " + std::to_string(location.line()) + "\n\n" +
            message;
    Logger::log(LOG_ERROR, "Assert", assertMsg);

#ifdef DEBUG
    if (!Dialogs::popupErrorChoice(assertMsg + "\n\nPress OK to continue, CANCEL to break in debugger.", false, "Assertion Failed"))
        breakInDebugger();
#endif

    if (isFatal)
        abort();
}

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
