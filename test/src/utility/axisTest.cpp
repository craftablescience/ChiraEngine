#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <utility/math/axis.h>

using namespace chira;

TEST(axis, getAxisFromString) {
    EXPECT_EQ(getAxisFromString("X"), axis::X);
    EXPECT_EQ(getAxisFromString("Y"), axis::Y);
    EXPECT_EQ(getAxisFromString("Z"), axis::Z);
}

TEST(axis, getSignedAxisFromString) {
    EXPECT_EQ(getSignedAxisFromString("XP"), signedAxis::XP);
    EXPECT_EQ(getSignedAxisFromString("YP"), signedAxis::YP);
    EXPECT_EQ(getSignedAxisFromString("ZP"), signedAxis::ZP);
    EXPECT_EQ(getSignedAxisFromString("XN"), signedAxis::XN);
    EXPECT_EQ(getSignedAxisFromString("YN"), signedAxis::YN);
    EXPECT_EQ(getSignedAxisFromString("ZN"), signedAxis::ZN);
}

#pragma clang diagnostic pop
