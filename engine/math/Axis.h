#pragma once

#include <string_view>

namespace chira {

enum class Axis {
    X,
    Y,
    Z
};

enum class SignedAxis {
    XP,
    XN,
    YP,
    YN,
    ZP,
    ZN
};

Axis getAxisFromString(std::string_view str);
SignedAxis getSignedAxisFromString(std::string_view str);

}
