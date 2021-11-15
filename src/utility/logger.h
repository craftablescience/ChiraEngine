#pragma once

#include <string_view>
#include <iostream>
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

    class logger {
    public:
        static inline constexpr std::string_view INFO_PREFIX = "[*]";
        static inline constexpr std::string_view INFO_IMPORTANT_PREFIX = "[!]";
        static inline constexpr std::string_view OUTPUT_PREFIX = "[O]";
        static inline constexpr std::string_view WARNING_PREFIX = "[W]";
        static inline constexpr std::string_view ERROR_PREFIX = "[E]";
        static void log(const loggerType& type, const std::string& source, const std::string& message);
        static uuids::uuid addCallback(const std::function<void(const loggerType&,const std::string&,const std::string&)>& function);
        static void runLogHooks(const loggerType& type, const std::string& source, const std::string& message);
        static void removeCallback(const uuids::uuid& id);
    private:
        static inline std::unordered_map<uuids::uuid, std::function<void(const loggerType&,const std::string&,const std::string&)>> callbacks{};
    };
}
