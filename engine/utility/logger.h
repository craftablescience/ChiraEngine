#pragma once

#include <string_view>
#include <functional>
#include <unordered_map>
#include "uuidGenerator.h"

namespace chira {
    enum class LogType {
        INFO,           // white
        INFO_IMPORTANT, // green
        OUTPUT,         // blue
        WARNING,        // yellow
        ERROR           // red
    };

    constexpr std::string_view LOGGER_INFO_PREFIX = "[*]";
    constexpr std::string_view LOGGER_INFO_IMPORTANT_PREFIX = "[!]";
    constexpr std::string_view LOGGER_OUTPUT_PREFIX = "[O]";
    constexpr std::string_view LOGGER_WARNING_PREFIX = "[W]";
    constexpr std::string_view LOGGER_ERROR_PREFIX = "[E]";

    class Logger {
        using loggingCallback = std::function<void(LogType,const std::string&,const std::string&)>;
    public:
        static void log(LogType type, const std::string& source, const std::string& message);
        static uuids::uuid addCallback(const loggingCallback& callback);
        static void runLogHooks(LogType type, const std::string& source, const std::string& message);
        static void removeCallback(const uuids::uuid& id);
    private:
        static inline std::unordered_map<uuids::uuid, loggingCallback> callbacks;
    };
}
