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

void remove(std::string& input, char charToRemove);

[[nodiscard]] std::vector<std::string> split(std::string_view input, char delimiter);

[[nodiscard]] std::string toLower(std::string_view str);
[[nodiscard]] std::string toUpper(std::string_view str);

[[nodiscard]] std::string stripLeft(const std::string& s);
[[nodiscard]] std::string stripRight(const std::string& s);
[[nodiscard]] std::string strip(const std::string& s);

[[nodiscard]] std::string stripLeft(const std::string& s, std::string_view c);
[[nodiscard]] std::string stripRight(const std::string& s, std::string_view c);
[[nodiscard]] std::string strip(const std::string& s, std::string_view c);

[[nodiscard]] std::string stripLeft(const std::string& s, char c);
[[nodiscard]] std::string stripRight(const std::string& s, char c);
[[nodiscard]] std::string strip(const std::string& s, char c);

void replace(std::string& s, char from, char to);
void replace(std::string& s, std::string_view from, std::string_view to);

[[nodiscard]] constexpr std::string_view getExtension(std::string_view filename) {
    auto dot = filename.find_last_of('.');
    auto sls = filename.find_last_of('/');
#ifdef CHIRA_PLATFORM_WINDOWS
    if (auto sls2 = filename.find_last_of('\\'); sls2 != std::string_view::npos && (sls == std::string_view::npos || sls2 > sls)) {
        sls = sls2;
    }
#endif
    if (dot != std::string_view::npos && (sls == std::string_view::npos || dot > sls) && dot < filename.size() - 1) {
        return filename.substr(dot + 1);
    }
    return "";
}

template<typename T>
requires requires (const T& t) {
    std::cbegin(t);
    std::cend(t);
}
[[nodiscard]] std::string join(const T& vector, std::string_view separator, std::string_view end = "") {
    auto first = std::cbegin(vector);
    auto last = std::cend(vector);
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
