#pragma once

#include <string>

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

    Axis getAxisFromString(const std::string& str);
    SignedAxis getSignedAxisFromString(const std::string& str);
}
