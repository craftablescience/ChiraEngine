#include "stringContains.h"

bool chira::startsWith(const std::string& str, char beginning) {
    return (str.length() >= 1) && (str.at(0) == beginning);
}

bool chira::startsWith(const std::string& str, const std::string& beginning) {
    return (str.length() >= beginning.length()) && (str.compare(0, beginning.length(), beginning) == 0);
}

bool chira::endsWith(const std::string& str, char end) {
    return (str.length() >= 1) && (str.at(str.length() - 1) == end);
}

bool chira::endsWith(const std::string& str, const std::string& end) {
    return (str.length() >= end.length()) && (str.compare(str.length() - end.length(), end.length(), end) == 0);
}

bool chira::contains(const std::string& str, char sub) {
    return (str.length() >= 1) && (str.find(sub) != std::string::npos);
}

bool chira::contains(const std::string& str, const std::string& substr) {
    return (str.length() >= substr.length()) && (str.find(substr) != std::string::npos);
}
