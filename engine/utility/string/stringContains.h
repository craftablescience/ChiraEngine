#pragma once

#include <string>

namespace chira {
    bool startsWith(const std::string& str, char beginning);
    bool startsWith(const std::string& str, const std::string& beginning);
    bool endsWith(const std::string& str, char end);
    bool endsWith(const std::string& str, const std::string& end);
    bool contains(const std::string& str, char sub);
    bool contains(const std::string& str, const std::string& substr);
}
