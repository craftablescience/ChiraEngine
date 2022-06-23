#include "Axis.h"

#include <i18n/TranslationManager.h>

chira::Axis chira::getAxisFromString(const std::string& str) {
    if (str == "X")
        return Axis::X;
    else if (str == "Y")
        return Axis::Y;
    else if (str == "Z")
        return Axis::Z;
    else {
        runtime_assert(false, TRF("error.axis.invalid_value", str, "axis"));
        return Axis::Z;
    }
}

chira::SignedAxis chira::getSignedAxisFromString(const std::string& str) {
    if (str == "XP")
        return SignedAxis::XP;
    else if (str == "XN")
        return SignedAxis::XN;
    else if (str == "YP")
        return SignedAxis::YP;
    else if (str == "YN")
        return SignedAxis::YN;
    else if (str == "ZP")
        return SignedAxis::ZP;
    else if (str == "ZN")
        return SignedAxis::ZN;
    else {
        runtime_assert(false, TRF("error.axis.invalid_value", str, "signedAxis"));
        return SignedAxis::ZN;
    }
}
