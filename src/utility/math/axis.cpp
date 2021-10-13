#include "axis.h"

#include "../logger.h"
#include "../../i18n/translationManager.h"
#include <fmt/core.h>

chira::axis chira::getAxisFromString(const std::string& str) {
    if (str == "X") {
        return X;
    } else if (str == "Y") {
        return Y;
    } else if (str == "Z") {
        return Z;
    } else {
        logger::log(ERR, "axis::getAxisFromString", fmt::format(TR("error.axis.invalid_value"), str, "axis"));
        return (axis) ((int) -1); // Uh oh
    }
}

chira::signedAxis chira::getSignedAxisFromString(const std::string& str) {
    if (str == "XP") {
        return XP;
    } else if (str == "XN") {
        return XN;
    } else if (str == "YP") {
        return YP;
    } else if (str == "YN") {
        return YN;
    } else if (str == "ZP") {
        return ZP;
    } else if (str == "ZN") {
        return ZN;
    } else {
        logger::log(ERR, "axis::getSignedAxisFromString", fmt::format(TR("error.axis.invalid_value"), str, "signedAxis"));
        return (signedAxis) ((int) -1); // Uh oh
    }
}
