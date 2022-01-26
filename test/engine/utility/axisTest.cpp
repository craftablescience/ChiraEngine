#include <gtest/gtest.h>

#include <utility/math/axis.h>

using namespace chira;

TEST(axis, getAxisFromString) {
    EXPECT_EQ(getAxisFromString("X"), Axis::X);
    EXPECT_EQ(getAxisFromString("Y"), Axis::Y);
    EXPECT_EQ(getAxisFromString("Z"), Axis::Z);
}

TEST(axis, getSignedAxisFromString) {
    EXPECT_EQ(getSignedAxisFromString("XP"), SignedAxis::XP);
    EXPECT_EQ(getSignedAxisFromString("YP"), SignedAxis::YP);
    EXPECT_EQ(getSignedAxisFromString("ZP"), SignedAxis::ZP);
    EXPECT_EQ(getSignedAxisFromString("XN"), SignedAxis::XN);
    EXPECT_EQ(getSignedAxisFromString("YN"), SignedAxis::YN);
    EXPECT_EQ(getSignedAxisFromString("ZN"), SignedAxis::ZN);
}
