#pragma once

#include <string_view>

namespace chira {
    constexpr int GL_VERSION_MAJOR = 4;
    constexpr int GL_VERSION_MINOR = 5;
    constexpr std::string_view GL_VERSION_STRING = "#version 450 core";
    constexpr std::string_view GL_VERSION_STRING_PRETTY = "4.5 Core";
}
