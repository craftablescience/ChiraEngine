#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <core/CommandLine.h>

using namespace chira;

TEST(CommandLine, has) {
    const char* const argv[] = {"ChiraTest", "foo"};
    PREINIT_ENGINE_WITH_ARGS(2, argv);

    EXPECT_TRUE(CommandLine::has("foo"));
    EXPECT_FALSE(CommandLine::has("bar"));
}

TEST(CommandLine, get) {
    const char* const argv[] = {"ChiraTest", "foo", "bar"};
    PREINIT_ENGINE_WITH_ARGS(3, argv);

    EXPECT_TRUE(CommandLine::has("foo"));
    EXPECT_TRUE(CommandLine::has("bar"));
    EXPECT_STREQ(CommandLine::get("foo").data(), "bar");
}

TEST(CommandLine, getProgramName) {
    const char* const argv[] = {"ChiraTest"};
    PREINIT_ENGINE_WITH_ARGS(1, argv);

    EXPECT_TRUE(CommandLine::has("ChiraTest"));
    EXPECT_STREQ(CommandLine::getProgramName().data(), "ChiraTest");
}
