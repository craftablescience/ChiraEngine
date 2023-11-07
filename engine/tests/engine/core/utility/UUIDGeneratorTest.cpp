#include <gtest/gtest.h>

#include <core/utility/UUIDGenerator.h>

using namespace chira;

TEST(UUIDGenerator, getNewUUID) {
    EXPECT_FALSE(UUIDGenerator::getNewUUID().is_nil());
}

TEST(UUIDGenerator, getNewUUIDString) {
    EXPECT_EQ(UUIDGenerator::getNewUUIDString().length(), 36);
}
