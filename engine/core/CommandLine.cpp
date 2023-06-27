#include "CommandLine.h"

#include <algorithm>
#include <vector>
#include <config/ConEntry.h>
#include "Logger.h"

using namespace chira;

CHIRA_CREATE_LOG(COMMANDLINE);

// todo(cmd): glob string values like "hello there" into one value minus quotes

std::vector<std::string_view> g_Arguments{""};

static inline void processFlag(int& start, int argc, const char* const argv[]) {
    g_Arguments.emplace_back(argv[start++]);
    if (start < argc) {
        g_Arguments.emplace_back(argv[start++]);
    }
}

static void processConEntry(int& i, int argc, const char* const argv[]) {
    std::string_view conEntry = argv[i];
    conEntry.remove_prefix(1);
    if (auto* command = ConEntryRegistry::getConCommand(conEntry)) {
        i++;
        std::vector<std::string> input;
        while (i < argc) {
            std::string_view current = argv[i];
            if (current.starts_with('-') || current.starts_with('+')) {
                break;
            }
            input.emplace_back(current.data());
        }
        command->fire(input);
    } else if (auto* var = ConEntryRegistry::getConVar(conEntry)) {
        i++;
        std::string value = argv[i];
        if (value.starts_with('-') || value.starts_with('+')) {
            LOG_COMMANDLINE.warning("ConVar \"{}\" not given a value!", conEntry);
        } else {
            if (var->hasFlag(CON_FLAG_READONLY)) {
                LOG_COMMANDLINE.warning("ConVar \"{}\" is read-only!", conEntry);
            } else {
                var->setValue(value);
            }
            i++;
        }
    } else {
        LOG_COMMANDLINE.warning("Unknown console entry \"{}\"", conEntry);
        i++;
    }
}

void CommandLine::init(int argc, const char* const argv[]) {
    if (!argc)
        return;
    g_Arguments[0] = argv[0];

    for (int i = 1; i < argc;) {
        std::string_view arg{argv[i]};
        if (arg.starts_with('-')) {
            processFlag(i, argc, argv);
        } else if (arg.starts_with('+')) {
            processConEntry(i, argc, argv);
        } else {
            i++;
        }
    }
}

bool CommandLine::has(std::string_view argument) {
    if (g_Arguments.size() <= 1) {
        return false;
    }
    return std::find(g_Arguments.begin() + 1, g_Arguments.end(), argument) != g_Arguments.end();
}

std::string_view CommandLine::get(std::string_view argument) {
    for (unsigned int i = 0; i < g_Arguments.size() - 1; i++) {
        if (argument == g_Arguments.at(i)) {
            return g_Arguments.at(i + 1);
        }
    }
    return "";
}

[[nodiscard]] std::string_view CommandLine::getOr(std::string_view argument, std::string_view default_) {
    auto value = get(argument);
    if (value.empty())
        return default_;
    return value;
}

std::string_view CommandLine::getProgramName() {
    return g_Arguments.at(0);
}
