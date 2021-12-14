#pragma once

#include <string_view>
#include <functional>
#include <unordered_map>
#include "uuidGenerator.h"

namespace chira {
    enum loggerType {
        INFO,           // white
        INFO_IMPORTANT, // green
        OUTPUT,         // blue
        WARN,        // yellow
        ERR           // red
    };

    constexpr std::string_view LOGGER_INFO_PREFIX = "[*]";
    constexpr std::string_view LOGGER_INFO_IMPORTANT_PREFIX = "[!]";
    constexpr std::string_view LOGGER_OUTPUT_PREFIX = "[O]";
    constexpr std::string_view LOGGER_WARNING_PREFIX = "[W]";
    constexpr std::string_view LOGGER_ERROR_PREFIX = "[E]";

    class logger {
    public:
        static void log(const loggerType& type, const std::string& source, const std::string& message);
        static uuids::uuid addCallback(const std::function<void(const loggerType&,const std::string&,const std::string&)>& function);
        static void runLogHooks(const loggerType& type, const std::string& source, const std::string& message);
        static void removeCallback(const uuids::uuid& id);
    private:
        static std::unordered_map<uuids::uuid, std::function<void(const loggerType&,const std::string&,const std::string&)>> callbacks;
    };
}
