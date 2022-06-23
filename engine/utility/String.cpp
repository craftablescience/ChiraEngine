#include "String.h"

#include <algorithm>
#include <sstream>
#include <regex>

using namespace chira;

bool String::startsWith(const std::string& str, char beginning) {
    return (str.length() >= 1) && (str.at(0) == beginning);
}

bool String::startsWith(const std::string& str, const std::string& beginning) {
    return (str.length() >= beginning.length()) && (str.compare(0, beginning.length(), beginning) == 0);
}

bool String::endsWith(const std::string& str, char end) {
    return (str.length() >= 1) && (str.at(str.length() - 1) == end);
}

bool String::endsWith(const std::string& str, const std::string& end) {
    return (str.length() >= end.length()) && (str.compare(str.length() - end.length(), end.length(), end) == 0);
}

bool String::contains(const std::string& str, char sub) {
    return (str.length() >= 1) && (str.find(sub) != std::string::npos);
}

bool String::contains(const std::string& str, const std::string& substr) {
    return (str.length() >= substr.length()) && (str.find(substr) != std::string::npos);
}

void String::remove(std::string& input, char charToRemove) {
    input.erase(std::remove(input.begin(), input.end(), charToRemove), input.end());
}

std::vector<std::string> String::split(const std::string& input, char delimiter) {
    std::stringstream ss{input};
    std::vector<std::string> out;
    std::string token;
    while (std::getline(ss, token, delimiter))
        out.push_back(token);
    return out;
}

// The following strip functions are modified from https://linuxhint.com/trim-string-cpp

std::string String::stripLeft(const std::string& s) {
    return std::regex_replace(s, std::regex("^[ \t\n\r\v\f]*"), "");
}

std::string String::stripRight(const std::string& s) {
    return std::regex_replace(s, std::regex("[ \t\n\r\v\f]*$"), "");
}

std::string String::strip(const std::string& s) {
    return std::regex_replace(s, std::regex("^[ \t\n\r\v\f]*|[ \t\n\r\v\f]*$"), "");
}

std::string String::stripLeft(const std::string& s, const std::string& c) {
    return std::regex_replace(s, std::regex("^[" + c +"]*"), "");
}

std::string String::stripRight(const std::string& s, const std::string& c) {
    return std::regex_replace(s, std::regex("[" + c +"]*$"), "");
}

std::string String::strip(const std::string& s, const std::string& c) {
    return std::regex_replace(s, std::regex("^[" + c + "]*|[" + c + "]*$"), "");
}

std::string String::stripLeft(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("^[" + std::to_string(c) +"]*"), "");
}

std::string String::stripRight(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("[" + std::to_string(c) +"]*$"), "");
}

std::string String::strip(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("^[" + std::to_string(c) + "]*|[" + c + "]*$"), "");
}
