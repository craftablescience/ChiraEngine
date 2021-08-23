#pragma once

#include <string_view>

namespace chira {
    const int GL_VERSION_MAJOR = 4;
    const int GL_VERSION_MINOR = 6;
    constexpr std::string_view GL_VERSION_STRING = "#version 460 core";
    constexpr std::string_view GL_VERSION_STRING_PRETTY = "4.6 Core";
}
