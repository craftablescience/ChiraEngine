#include "stringSplit.h"

#include <sstream>

std::vector<std::string> chira::split(const std::string& input, char delimiter) {
    std::stringstream ss{input};
    std::vector<std::string> out;
    std::string token;
    while (std::getline(ss, token, delimiter))
        out.push_back(token);
    return out;
}
