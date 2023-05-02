#pragma once

#include <string_view>

namespace chira::CommandLine {

/// Initialization is only meant to happen once per program instantiation.
void init(int argc, const char* const argv[]);
[[nodiscard]] bool has(std::string_view argument);
[[nodiscard]] std::string_view get(std::string_view argument);
[[nodiscard]] std::string_view getOr(std::string_view argument, std::string_view default_);
[[nodiscard]] std::string_view getProgramName();

} // namespace chira::CommandLine
