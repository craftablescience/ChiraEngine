#include "logger.h"

#include <iostream>

using namespace chira;

void logger::log(const loggerType& type, const std::string& source, const std::string& message) {
    switch (type) {
        case INFO:
            std::cout << INFO_PREFIX << "[" << source << "] " << message << '\n';
            break;
        case INFO_IMPORTANT:
            std::cout << "\x1B[32m" << INFO_IMPORTANT_PREFIX << "[" << source << "] " << message << "\033[0m" << '\n';
            break;
        case OUTPUT:
            std::cout << "\x1B[34m" << OUTPUT_PREFIX << "[" << source << "] " << message << "\033[0m" << '\n';
            break;
        case WARN:
            std::cout << "\x1B[33m" << WARNING_PREFIX << "[" << source << "] " << message << "\033[0m" << '\n';
            break;
        case ERR:
            std::cout << "\x1B[31m" << ERROR_PREFIX << "[" << source << "] " << message << "\033[0m" << '\n';
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
