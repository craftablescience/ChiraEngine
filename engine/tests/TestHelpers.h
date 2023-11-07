#pragma once

#include <core/debug/Logger.h>
#include <platforms/Common.h>

#define PREINIT_ENGINE() \
    const char* argv[] = {"ChiraTest"}; \
    chira::Engine::preinit(sizeof(argv) / sizeof(argv[0]), argv); \
    chira::Resource::addResourceProvider(new chira::FilesystemResourceProvider{"tests"})

#define PREINIT_ENGINE_WITH_ARGS(argv) \
    chira::Engine::preinit(sizeof(argv) / sizeof((argv)[0]), argv); \
    chira::Resource::addResourceProvider(new chira::FilesystemResourceProvider{"tests"})

#define LOG_BEGIN() \
    std::vector<chira::LogType> logMessageTypes; \
    std::vector<std::string>    logMessageSources; \
    std::vector<std::string>    logMessageMessages; \
    uuids::uuid logMessageID = chira::Logger::addCallback( \
            [&logMessageTypes, &logMessageSources, &logMessageMessages] \
            (chira::LogType type, std::string_view source, std::string_view message) { \
        logMessageTypes.push_back(type); \
        logMessageSources.emplace_back(source.data()); \
        logMessageMessages.emplace_back(message.data()); \
    }); \
    logMessageTypes.push_back(chira::LogType::LOG_ERROR); \
    logMessageSources.emplace_back(""); \
    logMessageMessages.emplace_back("")

#define LOG_TYPES_LIST    logMessageTypes
#define LOG_SOURCES_LIST  logMessageTypes
#define LOG_MESSAGES_LIST logMessageTypes

#define LOG_IS_NOT_EMPTY !logMessageMessages.empty()

#define LOG_LAST_TYPE    logMessageTypes[logMessageTypes.size() - 1]
#define LOG_LAST_SOURCE  logMessageSources[logMessageSources.size() - 1].c_str()
#define LOG_LAST_MESSAGE logMessageMessages[logMessageMessages.size() - 1].c_str()

#define LOG_HAS_TYPE(t) \
    [&] { \
        bool exists = false; \
        for (const auto& type : logMessageTypes) { \
            if (type == (t)) exists = true; \
        } \
        return exists; \
    }()

#define LOG_HAS_SOURCE(s) \
    [&] { \
        bool exists = false; \
        for (const auto& src : logMessageSources) { \
            if (src == (s)) exists = true; \
        } \
        return exists; \
    }()

#define LOG_HAS_MESSAGE(m) \
    [&] { \
        bool exists = false; \
        for (const auto& msg : logMessageMessages) { \
            if (msg == (m)) exists = true; \
        } \
        return exists; \
    }()

#define LOG_CLEAR() \
    logMessageTypes.clear(); \
    logMessageSources.clear(); \
    logMessageMessages.clear()

#define LOG_END() \
    chira::Logger::removeCallback(logMessageID)
