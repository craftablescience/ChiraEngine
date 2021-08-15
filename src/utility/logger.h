#pragma once

#include <string>
#include <iostream>
#include <functional>

enum loggerType {
    INFO,           // white
    INFO_IMPORTANT, // green
    OUTPUT,         // blue
    WARN,        // yellow
    ERR           // red
};

class chiraLogger {
public:
    static inline std::string INFO_PREFIX = "[*]";
    static inline std::string INFO_IMPORTANT_PREFIX = "[!]";
    static inline std::string OUTPUT_PREFIX = "[O]";
    static inline std::string WARNING_PREFIX = "[W]";
    static inline std::string ERROR_PREFIX = "[E]";
    static void log(const loggerType& type, const std::string& source, const std::string& message);
    static void addCallback(const std::function<void(const loggerType,const std::string&,const std::string&)>& function);
    static void runLogHooks(const loggerType& type, const std::string& source, const std::string& message);
private:
    static inline std::vector<std::function<void(const loggerType,const std::string&,const std::string&)>> callbacks{};
};
