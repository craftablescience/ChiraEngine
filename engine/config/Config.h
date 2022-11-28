#pragma once

#include <string_view>

namespace chira::Config {

constexpr int GL_VERSION_MAJOR = 4;
constexpr int GL_VERSION_MINOR = 1;
constexpr std::string_view GL_VERSION_STRING = "#version 410 core";
constexpr std::string_view GL_VERSION_STRING_PRETTY = "4.1 Core";

[[nodiscard]] std::string_view getConfigDirectory();

} // namespace chira::Config
