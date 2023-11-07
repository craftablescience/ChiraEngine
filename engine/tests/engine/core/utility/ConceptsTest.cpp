#include <gtest/gtest.h>

#include <core/utility/Concepts.h>

using namespace chira;

TEST(Concepts, arithmetic) {
    EXPECT_TRUE(CArithmetic<bool>);
    EXPECT_TRUE(CArithmetic<int>);
    EXPECT_TRUE(CArithmetic<float>);

    EXPECT_FALSE(CNonArithmetic<bool>);
    EXPECT_FALSE(CNonArithmetic<int>);
    EXPECT_FALSE(CNonArithmetic<float>);
}
