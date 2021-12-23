#include "stringRemove.h"

#include <algorithm>

void chira::remove(std::string& input, char charToRemove) {
    input.erase(std::remove(input.begin(), input.end(), charToRemove), input.end());
}
