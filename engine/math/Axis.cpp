#include "Axis.h"

#include <i18n/TranslationManager.h>

namespace chira {

Axis getAxisFromString(std::string_view str) {
    if (str == "X")
        return Axis::X;
    else if (str == "Y")
        return Axis::Y;
    else if (str == "Z")
        return Axis::Z;
    else {
        runtime_assert(false, "This string cannot be converted to Axis!");
        return Axis::Z;
    }
}

SignedAxis getSignedAxisFromString(std::string_view str) {
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
        runtime_assert(false, "This string cannot be converted to SignedAxis!");
        return SignedAxis::ZN;
    }
}

} // namespace chira
