#include "CommandLine.h"

#include <algorithm>
#include <filesystem>
#include <vector>

#include "config/ConEntry.h"
#include "debug/Logger.h"

using namespace chira;

CHIRA_CREATE_LOG(COMMANDLINE);

constexpr char OPTION_PREFIX = '-';
constexpr char CONVAR_PREFIX = '+';

std::vector<std::string_view> g_Arguments{};

static inline void processConEntry(int& i, int argc, const char* argv[]) {
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

void CommandLine::init(int argc, const char* argv[]) {
    if (!argc)
        return;
    g_Arguments.emplace_back(argv[0]);

    for (int i = 1; i < argc;) {
        std::string_view arg{argv[i]};
        if (arg.starts_with(CONVAR_PREFIX)) {
            processConEntry(i, argc, argv);
        } else {
            g_Arguments.push_back(arg);
            ++i;

            // If it starts with a dash, it's looking for an argument. However, if there's a
            // dash or plus sign prefixing the next arg there is no argument, so add a blank
            if (i == argc) {
                g_Arguments.emplace_back("");
            } else if (arg.starts_with(OPTION_PREFIX)) {
                std::string_view nextArg{argv[i]};
                if (nextArg.starts_with(OPTION_PREFIX) || nextArg.starts_with(CONVAR_PREFIX)) {
                    g_Arguments.emplace_back("");
                }
            }
        }
    }
}

std::string_view CommandLine::at(unsigned int index) {
	if (g_Arguments.size() <= index) {
		return "";
	}
	return g_Arguments.at(index);
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

std::string_view CommandLine::getOr(std::string_view argument, std::string_view default_) {
    auto value = get(argument);
    if (value.empty())
        return default_;
    return value;
}

bool CommandLine::hasDefaultArgument() {
    return g_Arguments.size() >= 2 && !g_Arguments[1].starts_with(OPTION_PREFIX) && !g_Arguments[1].starts_with(CONVAR_PREFIX);
}

std::string_view CommandLine::getDefaultArgument() {
    if (CommandLine::hasDefaultArgument())
        return g_Arguments[1];
    return "";
}

std::string_view CommandLine::getDefaultArgumentOr(std::string_view default_) {
    if (CommandLine::hasDefaultArgument())
        return g_Arguments[1];
    return default_;
}

std::string_view CommandLine::getProgramName() {
	static std::string programName = std::filesystem::path{g_Arguments.at(0)}.stem().string();
    return programName;
}
