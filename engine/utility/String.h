#pragma once

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace chira::String {

[[nodiscard]] constexpr char toLowerChar(char in) {
    if (in >= 'A' && in <= 'Z') {
        return static_cast<char>(in + 32);
    }
    return in;
}

[[nodiscard]] constexpr char toUpperChar(char in) {
    if (in >= 'a' && in <= 'z') {
        return static_cast<char>(in - 32);
    }
    return in;
}

bool startsWith(std::string_view str, char beginning);
bool startsWith(std::string_view str, std::string_view beginning);

bool endsWith(std::string_view str, char end);
bool endsWith(std::string_view str, std::string_view end);

bool contains(std::string_view str, char sub);
bool contains(std::string_view str, std::string_view substr);

void remove(std::string& input, char charToRemove);

std::vector<std::string> split(const std::string& input, char delimiter);

std::string toLower(std::string_view str);
std::string toUpper(std::string_view str);

std::string stripLeft(const std::string& s);
std::string stripRight(const std::string& s);
std::string strip(const std::string& s);

std::string stripLeft(const std::string& s, std::string_view c);
std::string stripRight(const std::string& s, std::string_view c);
std::string strip(const std::string& s, std::string_view c);

std::string stripLeft(const std::string& s, char c);
std::string stripRight(const std::string& s, char c);
std::string strip(const std::string& s, char c);

void replace(std::string& s, std::string_view from, std::string_view to);

template<typename T>
requires requires (T t) {
    std::begin(t);
    std::end(t);
}
std::string join(T& vector, std::string_view separator, std::string_view end = "") {
    auto first = std::begin(vector);
    auto last = std::end(vector);
    if (first == last) {
        return end.data();
    }

    std::stringstream ss;
    ss << *first;
    ++first;

    while (first != last) {
        ss << separator;
        ss << *first;
        ++first;
    }
    ss << end;

    return ss.str();
}

} // namespace chira::String
