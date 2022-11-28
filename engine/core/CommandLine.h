#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace chira {

class CommandLine {
    friend class Engine;
private:
    static void initialize(int argc, const char* const argv[]);
public:
    CommandLine() = delete;

    [[nodiscard]] static bool has(std::string_view argument);
    [[nodiscard]] static std::string_view get(std::string_view argument);
    [[nodiscard]] static std::string_view getProgramName();
private:
    static inline std::vector<std::string> arguments;
};

} // namespace chira
