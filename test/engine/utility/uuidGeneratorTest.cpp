#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>

#include <utility/uuidGenerator.h>

using namespace chira;

TEST(uuidGenerator, getNewUUID) {
    EXPECT_FALSE(uuidGenerator::getNewUUID().is_nil());
}

TEST(uuidGenerator, getNewUUIDString) {
    EXPECT_EQ(uuidGenerator::getNewUUIDString().length(), 36);
}

#pragma clang diagnostic pop
