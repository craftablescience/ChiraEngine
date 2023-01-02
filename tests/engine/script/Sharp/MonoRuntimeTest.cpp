#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <script/Sharp/MonoRuntime.h>
#include <mono/metadata/debug-helpers.h>

using namespace chira;

TEST(MonoRuntime, helloWorld) {
    PREINIT_ENGINE();
    SETUP_SCRIPTENGINE();

    LOG_BEGIN();
    
    MonoAssembly *chiratest = mono_domain_assembly_open(MonoRuntime::getDomain(), "ChiraTest.dll");
    if (!chiratest)
        throw std::runtime_error("Failed to open test assembly");
    MonoImage *testimg = mono_assembly_get_image(chiratest);
    MonoClass *testclass = mono_class_from_name(testimg, "ChiraTest", "TestEnt");
    MonoObject *obj = mono_object_new(MonoRuntime::getDomain(), testclass);
    mono_runtime_object_init(obj);
    MonoClass *klass = mono_object_get_class(obj);
    MonoMethodDesc *mdesc = mono_method_desc_new (":SpeakHello()", 0);
    MonoMethod *method = mono_method_desc_search_in_class(mdesc, klass);
    mono_runtime_invoke(method, obj, NULL, NULL);
    mono_method_desc_free(mdesc);

    ASSERT_TRUE(LOG_IS_NOT_EMPTY);
    EXPECT_EQ(LOG_LAST_TYPE, LogType::LOG_OUTPUT);
    EXPECT_STREQ(LOG_LAST_SOURCE, "MONO");
    EXPECT_STREQ(LOG_LAST_MESSAGE, "Hello World!");
    LOG_END();

    SHUTDOWN_SCRIPTENGINE();
}