#pragma once

#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <script/AngelScriptVM.h>
#include <utility/Logger.h>

#define SETUP_ENGINE() \
    Engine::preInit("config_test.json"); \
    Resource::addResourceProvider(new FilesystemResourceProvider{"tests"})

#define SETUP_ANGELSCRIPT() AngelScriptVM::init()

#define LOGGING_BEGIN() \
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

#define LOG_GET_LAST_TYPE()    logMessageTypes[logMessageTypes.size() - 1]
#define LOG_GET_LAST_SOURCE()  logMessageSources[logMessageSources.size() - 1].c_str()
#define LOG_GET_LAST_MESSAGE() logMessageMessages[logMessageMessages.size() - 1].c_str()

#define LOGGING_END() \
    chira::Logger::removeCallback(logMessageID)
