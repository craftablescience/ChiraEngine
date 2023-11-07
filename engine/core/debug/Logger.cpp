#include "Logger.h"

#include <iostream>
#include <ctime>

#include "../config/ConEntry.h"
#include "../Platform.h"

using namespace chira;

ConVar log_timestamp{"log_timestamp", true, "Print the timestamp of a console message in the terminal.", CON_FLAG_CACHE};
ConVar log_source{"log_source", true, "Print the source of a console message in the terminal and console panel.", CON_FLAG_CACHE};

// Using std::endl here instead of '\n' in debug
// sometimes with '\n', the buffer doesn't get flushed until the application exits
#ifdef DEBUG
    #define CHIRA_LOGGER_SUFFIX std::endl
#else
    #define CHIRA_LOGGER_SUFFIX '\n'
#endif

void Logger::log(LogType type, std::string_view source, std::string_view message) {
    std::string logTime;
    if (log_timestamp.getValue<bool>()) {
        std::time_t now = std::time(nullptr);
        logTime += '[';
        char timeStr[64];
#ifdef CHIRA_COMPILER_MSVC
        // WHY MICROSOFT WHY
        tm timeLocal{};
        localtime_s(&timeLocal, &now);
        std::strftime(timeStr, sizeof(timeStr), "%c", &timeLocal);
#else
        std::strftime(timeStr, sizeof(timeStr), "%c", std::localtime(&now));
#endif
        logTime += timeStr;
        logTime += ']';
    }

    std::string logSource;
    if (log_source.getValue<bool>()) {
        logSource += '[';
        logSource += source.data();
        logSource += ']';
    }
    switch (type) {
        using enum LogType;
        case LOG_INFO:
            std::cout << logTime << Logger::INFO_PREFIX << logSource << " " << message << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_INFO_IMPORTANT:
            std::cout << "\x1B[32m" << logTime << Logger::INFO_IMPORTANT_PREFIX << logSource << " " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_OUTPUT:
            std::cout << "\x1B[34m" << logTime << Logger::OUTPUT_PREFIX << logSource << " " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_WARNING:
            std::cout << "\x1B[33m" << logTime << Logger::WARNING_PREFIX << logSource << " " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
            break;
        case LOG_ERROR:
            std::cout << "\x1B[31m" << logTime << Logger::ERROR_PREFIX << logSource << " " << message << "\033[0m" << CHIRA_LOGGER_SUFFIX;
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
