#include "CommandLine.h"

#include <vector>

using namespace chira;

// todo(cmd): set convars in init if option starts with a +
// todo(cmd): glob string values like "hello there" into one value minus quotes

std::vector<const char*> arguments{};

void CommandLine::init(int argc, const char* const argv[]) {
    for (int i = 0; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }
}

bool CommandLine::has(std::string_view argument) {
    return std::find(arguments.begin(), arguments.end(), argument) != arguments.end();
}

std::string_view CommandLine::get(std::string_view argument) {
    for (int i = 0; i < arguments.size() - 1; i++) {
        if (argument == arguments.at(i)) {
            return arguments.at(i + 1);
        }
    }
    return "";
}

std::string_view CommandLine::getProgramName() {
    if (!arguments.empty()) {
        return arguments.at(0);
    }
    return "";
}
