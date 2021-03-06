#include "Logger.h"

#include <iostream>
#include <config/ConEntry.h>

using namespace chira;

static ConVar log_print_source{"log_print_source", true, "Print the source of a console message.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

// Using std::endl here instead of '\n' in debug
// sometimes with '\n', the buffer doesn't get flushed until the application exits
#ifdef DEBUG
    #define CHIRA_LOGGER_SUFFIX std::endl
#else
    #define CHIRA_LOGGER_SUFFIX '\n'
#endif

void Logger::log(LogType type, std::string_view source, std::string_view message) {
#ifdef _WIN32
    static bool enabledWindowsColoredText = false;
    if (!enabledWindowsColoredText) {
        // Enable colored text in Windows console by setting encoding to UTF-8
        // #define CP_UTF8 65001 in windows.h
        system("chcp 65001 > nul");
        enabledWindowsColoredText = false;
    }
#endif

    std::string logSource{};
    if (log_print_source.getValue<bool>()) {
        logSource += "[";
        logSource += source.data();
        logSource += "]";
    }
    switch (type) {
        case LOG_INFO:
            std::cout << Logger::INFO_PREFIX << logSource << " " << message << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_INFO_IMPORTANT:
            std::cout << "\x1B[32m" << Logger::INFO_IMPORTANT_PREFIX << logSource << " " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_OUTPUT:
            std::cout << "\x1B[34m" << Logger::OUTPUT_PREFIX << logSource << " " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_WARNING:
            std::cout << "\x1B[33m" << Logger::WARNING_PREFIX << logSource << " " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_ERROR:
            std::cout << "\x1B[31m" << Logger::ERROR_PREFIX << logSource << " " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
    }
    Logger::runLogHooks(type, source, message);
}

uuids::uuid Logger::addCallback(const loggingCallback& callback) {
    auto id = UUIDGenerator::getNewUUID();
    Logger::callbacks[id] = callback;
    return id;
}

void Logger::runLogHooks(LogType type, std::string_view source, std::string_view message) {
    for (const auto& [id, callback] : Logger::callbacks) {
        callback(type, source, message);
    }
}

void Logger::removeCallback(const uuids::uuid& id) {
    Logger::callbacks.erase(id);
}
