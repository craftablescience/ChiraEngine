#pragma once

#include <string_view>
#include <functional>
#include <unordered_map>

#include "UUIDGenerator.h"

namespace chira {

enum LogType {
    LOG_INFO,           // white
    LOG_INFO_IMPORTANT, // green
    LOG_OUTPUT,         // blue
    LOG_WARNING,        // yellow
    LOG_ERROR           // red
};

class Logger {
    using loggingCallback = std::function<void(LogType,std::string_view,std::string_view)>;
public:
    static void log(LogType type, std::string_view source, std::string_view message);
    static uuids::uuid addCallback(const loggingCallback& callback);
    static void runLogHooks(LogType type, std::string_view source, std::string_view message);
    static void removeCallback(const uuids::uuid& id);

    static inline constexpr std::string_view INFO_PREFIX = "[*]";
    static inline constexpr std::string_view INFO_IMPORTANT_PREFIX = "[!]";
    static inline constexpr std::string_view OUTPUT_PREFIX = "[O]";
    static inline constexpr std::string_view WARNING_PREFIX = "[W]";
    static inline constexpr std::string_view ERROR_PREFIX = "[E]";
private:
    static inline std::unordered_map<uuids::uuid, loggingCallback> callbacks;
};

} // namespace chira
