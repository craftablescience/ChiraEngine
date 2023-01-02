#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <script/AS/AngelScriptHolder.h>

using namespace chira;

TEST(AngelScriptHolder, helloWorld) {
    PREINIT_ENGINE();
    SETUP_SCRIPTENGINE();

    AngelScriptHolder script{"file://scripts/test.as"};
    LOG_BEGIN();
    script.getFunction("main")();
    ASSERT_TRUE(LOG_IS_NOT_EMPTY);
    EXPECT_EQ(LOG_LAST_TYPE, LogType::LOG_OUTPUT);
    EXPECT_STREQ(LOG_LAST_SOURCE, "ANGELSCRIPTVM");
    EXPECT_STREQ(LOG_LAST_MESSAGE, "hello world");
    LOG_END();

    SHUTDOWN_SCRIPTENGINE();
}
