#include "CommandLine.h"

#include <config/ConEntry.h>

using namespace chira;

void CommandLine::initialize(int argc, const char* const argv[]) {
    for (int i = 0; i < argc; i++) {
        CommandLine::arguments.emplace_back(argv[i]);
    }
}

bool CommandLine::has(std::string_view argument) {
    return std::find(CommandLine::arguments.begin(), CommandLine::arguments.end(), argument) != CommandLine::arguments.end();
}

std::string_view CommandLine::get(std::string_view argument) {
    for (int i = 0; i < CommandLine::arguments.size() - 1; i++) {
        if (argument == CommandLine::arguments.at(i)) {
            return CommandLine::arguments.at(i + 1);
        }
    }
    return "";
}

std::string_view CommandLine::getProgramName() {
    if (!CommandLine::arguments.empty()) {
        return CommandLine::arguments.at(0);
    }
    return "";
}
