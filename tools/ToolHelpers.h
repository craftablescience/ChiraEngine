#pragma once

#include <string>
#include <core/Logger.h>

#define CHIRA_SETUP_CLI_TOOL(name, version, helpText) \
    CHIRA_CREATE_LOG(name);                             \
    static void printHelp() {                           \
        LOG_##name.infoImportant(                       \
            "\n" #name " v" version "\n" helpText       \
        );                                              \
    }                                                   \
    constexpr std::string_view name##_VERSION = version \
