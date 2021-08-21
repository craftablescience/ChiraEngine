#include "logger.h"

void chira::logger::log(const loggerType& type, const std::string& source, const std::string& message) {
    switch (type) {
        case INFO:
            std::cout << INFO_PREFIX << "[" << source << "] " << message << std::endl;
            break;
        case INFO_IMPORTANT:
            std::cout << "\x1B[32m" << INFO_IMPORTANT_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case OUTPUT:
            std::cout << "\x1B[34m" << OUTPUT_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case WARN:
            std::cout << "\x1B[33m" << WARNING_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case ERR:
            std::cout << "\x1B[31m" << ERROR_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
    }
    chira::logger::runLogHooks(type, source, message);
}

void chira::logger::addCallback(const std::function<void(const loggerType,const std::string&,const std::string&)>& function) {
    chira::logger::callbacks.push_back(function);
}

void chira::logger::runLogHooks(const loggerType& type, const std::string& source, const std::string& message) {
    for (const std::function<void(const loggerType,const std::string&,const std::string&)>& function : chira::logger::callbacks) {
        function(type, source, message);
    }
}
