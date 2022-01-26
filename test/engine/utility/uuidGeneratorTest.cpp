#include <gtest/gtest.h>

#include <utility/uuidGenerator.h>

using namespace chira;

TEST(uuidGenerator, getNewUUID) {
    EXPECT_FALSE(UUIDGenerator::getNewUUID().is_nil());
}

TEST(uuidGenerator, getNewUUIDString) {
    EXPECT_EQ(UUIDGenerator::getNewUUIDString().length(), 36);
}
