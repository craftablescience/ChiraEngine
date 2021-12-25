#include "logger.h"

#include <iostream>

using namespace chira;

std::unordered_map<uuids::uuid, std::function<void(const loggerType&,const std::string&,const std::string&)>> logger::callbacks{};

void logger::log(const loggerType& type, const std::string& source, const std::string& message) {
    switch (type) {
        // Using std::endl here instead of '\n'
        // sometimes with the latter, the buffer doesn't get flushed until the application exits
        case INFO:
            std::cout << LOGGER_INFO_PREFIX << "[" << source << "] " << message << std::endl;
            break;
        case INFO_IMPORTANT:
            std::cout << "\x1B[32m" << LOGGER_INFO_IMPORTANT_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case OUTPUT:
            std::cout << "\x1B[34m" << LOGGER_OUTPUT_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case WARN:
            std::cout << "\x1B[33m" << LOGGER_WARNING_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
        case ERR:
            std::cout << "\x1B[31m" << LOGGER_ERROR_PREFIX << "[" << source << "] " << message << "\033[0m" << std::endl;
            break;
    }
    logger::runLogHooks(type, source, message);
}

uuids::uuid logger::addCallback(const std::function<void(const loggerType&,const std::string&,const std::string&)>& function) {
    auto id = uuidGenerator::getNewUUID();
    logger::callbacks[id] = function;
    return id;
}

void logger::runLogHooks(const loggerType& type, const std::string& source, const std::string& message) {
    for (const auto& [id, function] : logger::callbacks) {
        function(type, source, message);
    }
}

void logger::removeCallback(const uuids::uuid& id) {
    logger::callbacks.erase(id);
}
