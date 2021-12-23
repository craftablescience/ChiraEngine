#include "axis.h"

#include <utility/logger.h>
#include <i18n/translationManager.h>

chira::axis chira::getAxisFromString(const std::string& str) {
    if (str == "X") {
        return axis::X;
    } else if (str == "Y") {
        return axis::Y;
    } else if (str == "Z") {
        return axis::Z;
    } else {
        logger::log(ERR, "axis::getAxisFromString", fmt::format(TR("error.axis.invalid_value"), str, "axis"));
        return axis::Z; // oh no
    }
}

chira::signedAxis chira::getSignedAxisFromString(const std::string& str) {
    if (str == "XP") {
        return signedAxis::XP;
    } else if (str == "XN") {
        return signedAxis::XN;
    } else if (str == "YP") {
        return signedAxis::YP;
    } else if (str == "YN") {
        return signedAxis::YN;
    } else if (str == "ZP") {
        return signedAxis::ZP;
    } else if (str == "ZN") {
        return signedAxis::ZN;
    } else {
        logger::log(ERR, "axis::getSignedAxisFromString", fmt::format(TR("error.axis.invalid_value"), str, "signedAxis"));
        return signedAxis::ZN; // oh no
    }
}
