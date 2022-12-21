#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <script/AngelScriptHolder.h>

using namespace chira;

TEST(AngelScriptHolder, print) {
    PREINIT_ENGINE();
    SETUP_ANGELSCRIPT();
    LOG_BEGIN();

    AngelScriptHolder script{"file://scripts/test.as"};

    script.callFunction<void>("testPrintHelloWorld");
    ASSERT_TRUE(LOG_IS_NOT_EMPTY);
    EXPECT_EQ(LOG_LAST_TYPE, LogType::LOG_OUTPUT);
    EXPECT_STREQ(LOG_LAST_SOURCE, "ANGELSCRIPTVM");
    EXPECT_STREQ(LOG_LAST_MESSAGE, "hello world");

    LOG_END();
}

TEST(AngelScriptHolder, callFuncVoidParamsNone) {
    PREINIT_ENGINE();
    SETUP_ANGELSCRIPT();
    LOG_BEGIN();

    AngelScriptHolder script{"file://scripts/test.as"};

    script.callFunction<void>("testVoidParamsNone");
    ASSERT_TRUE(LOG_IS_NOT_EMPTY);
    EXPECT_EQ(LOG_LAST_TYPE, LogType::LOG_OUTPUT);
    EXPECT_STREQ(LOG_LAST_SOURCE, "ANGELSCRIPTVM");
    EXPECT_STREQ(LOG_LAST_MESSAGE, "testVoidParamsNone");

    LOG_END();
}

TEST(AngelScriptHolder, callFuncReturnsParamsNone) {
    PREINIT_ENGINE();
    SETUP_ANGELSCRIPT();
    LOG_BEGIN();

    AngelScriptHolder script{"file://scripts/test.as"};

    EXPECT_TRUE(script.callFunction<bool>("testBoolParamsNone"));

    LOG_END();
}

TEST(AngelScriptHolder, callFuncVoidParamsVarious) {
    PREINIT_ENGINE();
    SETUP_ANGELSCRIPT();
    LOG_BEGIN();

    AngelScriptHolder script{"file://scripts/test.as"};

    script.callFunction<void>("testVoidParamsBool", true);
    ASSERT_TRUE(LOG_IS_NOT_EMPTY);
    EXPECT_EQ(LOG_LAST_TYPE, LogType::LOG_OUTPUT);
    EXPECT_STREQ(LOG_LAST_SOURCE, "ANGELSCRIPTVM");
    EXPECT_STREQ(LOG_LAST_MESSAGE, "testVoidParamsBool");

    LOG_END();
}

// todo(as): make more tests with all C++ primitives
// todo(as): make tests with std::string, objects, pointers when functionality exists
