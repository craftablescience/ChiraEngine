#include "stringRemove.h"

#include <algorithm>

void chira::remove(std::string& input, const std::string& strToRemove) {
    input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
}
