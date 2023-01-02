#include "MonoRuntime.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <script/Engine.h>
#include <config/ConEntry.h>

#include <mono/metadata/debug-helpers.h>

using namespace chira;

CHIRA_CREATE_LOG(MONO);

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
    LOG_MONO.output(mono_string_to_utf8(message));
}