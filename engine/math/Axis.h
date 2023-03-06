#pragma once

#include <string_view>

namespace chira {

enum class Axis {
    X,
    Y,
    Z,
};

enum class SignedAxis {
    XP,
    XN,
    YP,
    YN,
    ZP,
    ZN,
};

} // namespace chira
