#include "String.h"

#include <algorithm>
#include <regex>

using namespace chira;

void String::remove(std::string& input, char charToRemove) {
    input.erase(std::remove(input.begin(), input.end(), charToRemove), input.end());
}

std::vector<std::string> String::split(std::string_view input, char delimiter) {
    std::stringstream ss{input.data()};
    std::vector<std::string> out;
    std::string token;
    while (std::getline(ss, token, delimiter))
        out.push_back(token);
    return out;
}

std::string String::toLower(std::string_view str) {
    std::string out{str};
    std::transform(out.begin(), out.end(), out.begin(), toLowerChar);
    return out;
}

std::string String::toUpper(std::string_view str) {
    std::string out{str};
    std::transform(out.begin(), out.end(), out.begin(), toUpperChar);
    return out;
}

// The following strip functions are modified from https://linuxhint.com/trim-string-cpp

std::string String::stripLeft(const std::string& s) {
    static const auto regexStripLeft = std::regex("^[ \t\n\r\v\f]*");
    return std::regex_replace(s, regexStripLeft, "");
}

std::string String::stripRight(const std::string& s) {
    static const auto regexStripRight = std::regex("[ \t\n\r\v\f]*$");
    return std::regex_replace(s, regexStripRight, "");
}

std::string String::strip(const std::string& s) {
    static const auto regexStrip = std::regex("^[ \t\n\r\v\f]*|[ \t\n\r\v\f]*$");
    return std::regex_replace(s, regexStrip, "");
}

std::string String::stripLeft(const std::string& s, std::string_view c) {
    return std::regex_replace(s, std::regex(std::string{"^["} + c.data() +"]*"), "");
}

std::string String::stripRight(const std::string& s, std::string_view c) {
    return std::regex_replace(s, std::regex(std::string{"["} + c.data() +"]*$"), "");
}

std::string String::strip(const std::string& s, std::string_view c) {
    return std::regex_replace(s, std::regex(std::string{"^["} + c.data() + "]*|[" + c.data() + "]*$"), "");
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

void String::replace(std::string& s, char from, char to) {
    std::replace(s.begin(), s.end(), from, to);
}

void String::replace(std::string& s, std::string_view from, std::string_view to) {
    s = std::regex_replace(s, std::regex{from.data()}, to.data());
}
