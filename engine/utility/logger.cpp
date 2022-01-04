#include "logger.h"

#include <iostream>

using namespace chira;

void Logger::log(LogType type, const std::string& source, const std::string& message) {
    switch (type) {
        // Using std::endl here instead of '\n'
        // sometimes with the latter, the buffer doesn't get flushed until the application exits
        case LogType::INFO:
            std::cout << LOGGER_INFO_PREFIX << "[" << source << "] " << message << std::endl;
            break;
        case LogType::INFO_IMPORTANT:
            std::cout << "\x1B[32m" << LOGGER_INFO_IMPORTANT_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case LogType::OUTPUT:
            std::cout << "\x1B[34m" << LOGGER_OUTPUT_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case LogType::WARNING:
            std::cout << "\x1B[33m" << LOGGER_WARNING_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case LogType::ERROR:
            std::cout << "\x1B[31m" << LOGGER_ERROR_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
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
