#include <gtest/gtest.h>

#include <TestHelpers.h>
#include <script/AngelScriptHolder.h>

using namespace chira;

TEST(AngelScriptHolder, helloWorld) {
    SETUP_ENGINE();
    SETUP_ANGELSCRIPT();
    LOGGING_BEGIN();

    AngelScriptHolder script{"file://scripts/test.as"};
    script.getFunction("main")();
    ASSERT_EQ(LOG_GET_LAST_TYPE(), LogType::LOG_OUTPUT);
    ASSERT_STREQ(LOG_GET_LAST_SOURCE(), "AngelScript");
    ASSERT_STREQ(LOG_GET_LAST_MESSAGE(), "hello world");

    LOGGING_END();
}
