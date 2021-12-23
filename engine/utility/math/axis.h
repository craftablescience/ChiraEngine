#pragma once

#include <string>

namespace chira {
    enum class axis {
        X,
        Y,
        Z
    };

    enum class signedAxis {
        XP,
        XN,
        YP,
        YN,
        ZP,
        ZN
    };

    axis getAxisFromString(const std::string& str);
    signedAxis getSignedAxisFromString(const std::string& str);
}
