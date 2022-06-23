#include "Logger.h"

#include <iostream>

using namespace chira;

// Using std::endl here instead of '\n' in debug
// sometimes with '\n', the buffer doesn't get flushed until the application exits
#ifdef DEBUG
    #define CHIRA_LOGGER_SUFFIX std::endl
#else
    #define CHIRA_LOGGER_SUFFIX '\n'
#endif

void Logger::log(LogType type, const std::string& source, const std::string& message) {
    switch (type) {
        case LOG_INFO:
            std::cout << Logger::INFO_PREFIX << "[" << source << "] " << message << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_INFO_IMPORTANT:
            std::cout << "\x1B[32m" << Logger::INFO_IMPORTANT_PREFIX << "[" << source << "] " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_OUTPUT:
            std::cout << "\x1B[34m" << Logger::OUTPUT_PREFIX << "[" << source << "] " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_WARNING:
            std::cout << "\x1B[33m" << Logger::WARNING_PREFIX << "[" << source << "] " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_ERROR:
            std::cout << "\x1B[31m" << Logger::ERROR_PREFIX << "[" << source << "] " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
    }
    Logger::runLogHooks(type, source, message);
}

uuids::uuid Logger::addCallback(const loggingCallback& callback) {
    auto id = UUIDGenerator::getNewUUID();
    Logger::callbacks[id] = callback;
    return id;
}

void Logger::runLogHooks(LogType type, const std::string& source, const std::string& message) {
    for (const auto& [id, callback] : Logger::callbacks) {
        callback(type, source, message);
    }
}

void Logger::removeCallback(const uuids::uuid& id) {
    Logger::callbacks.erase(id);
}
