#pragma once

#include <string>

#include <core/debug/Logger.h>
#include <core/Platform.h>
#include <platforms/Common.h>

#define CHIRA_SETUP_CLI_TOOL(name, version, helpText) \
    CHIRA_CREATE_LOG(name);                           \
    static void printHelp() {                         \
        LOG_##name.infoImportant(                     \
            "\n" #name " v" version "\n" helpText     \
        );                                            \
    }                                                 \
    constexpr std::string_view name##_VERSION = version

#ifndef CHIRA_PLATFORM_WINDOWS
    #define CHIRA_SETUP_GUI_TOOL(name) \
        CHIRA_CREATE_LOG(name)
#else
    // Use the best available GPU on Windows
    #define CHIRA_SETUP_GUI_TOOL(name)                                             \
        extern "C" {                                                               \
            [[maybe_unused]]                                                       \
            __declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;        \
            [[maybe_unused]]                                                       \
            __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01; \
        }                                                                          \
        CHIRA_CREATE_LOG(name)
#endif
