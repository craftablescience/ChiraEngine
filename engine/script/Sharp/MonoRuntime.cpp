#include "MonoRuntime.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <script/Engine.h>
#include <config/ConEntry.h>

#include <mono/metadata/debug-helpers.h>

using namespace chira;

CHIRA_CREATE_LOG(MONO);

[[maybe_unused]]
static ConCommand test_mono{"test_mono", "tests mono runtime with ChiraTest.dll and a sample class", [] { // NOLINT(cert-err58-cpp)
    LOG_MONO.info("Loading ChiraTest Assembly...");
    MonoAssembly *chiratest = mono_domain_assembly_open(MonoRuntime::getDomain(), "ChiraTest.dll");
    if (!chiratest)
        throw std::runtime_error("Failed to open test assembly");

    LOG_MONO.info("mono_assembly_get_image(chiratest)");
    MonoImage *testimg = mono_assembly_get_image(chiratest);

    LOG_MONO.info("Attempting to get TestEnt Class");
    MonoClass *testclass = mono_class_from_name(testimg, "ChiraTest", "TestEnt");

    LOG_MONO.info("Initializing Object...");
    MonoObject *obj = mono_object_new(MonoRuntime::getDomain(), testclass);
    mono_runtime_object_init(obj);

    LOG_MONO.info("Invoking SpeakHello()...");
    MonoClass *klass = mono_object_get_class(obj);
    MonoMethodDesc *mdesc = mono_method_desc_new (":SpeakHello()", 0);
    MonoMethod *method = mono_method_desc_search_in_class(mdesc, klass);
    mono_runtime_invoke(method, obj, NULL, NULL);
    mono_method_desc_free(mdesc);
}};

static void print(MonoString* message);

void MonoRuntime::init() {
    // mono_config_parse (NULL);
    MonoRuntime::domain = mono_jit_init ("ChiraEngineMain");

    if (MonoRuntime::domain == nullptr) {
        throw std::runtime_error(TRC("error.monoruntime.jit_init_failure"));
    }

    // register some base calls
    mono_add_internal_call("ChiraSharp.CE::Print(string)", (const void*)print);

    // load in the base assembly
    MonoRuntime::ceassembly = mono_domain_assembly_open(MonoRuntime::domain, "ChiraSharp.dll");
}

void MonoRuntime::shutdown() {
    mono_jit_cleanup(MonoRuntime::domain);
}

void ScriptEngine::init() {
    MonoRuntime::init();
}

void ScriptEngine::shutdown() {
    MonoRuntime::shutdown();
}

static void print(MonoString* message) {
    LOG_MONO.info(mono_string_to_utf8(message));
}