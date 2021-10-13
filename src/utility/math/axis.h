#pragma once

#include <string>

namespace chira {
    enum axis {
        X,
        Y,
        Z
    };

    enum signedAxis {
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
