#include "AngelScriptHolder.h"

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

#include <resource/StringResource.h>
#include <i18n/TranslationManager.h>

using namespace chira;

AngelScriptHolder::AngelScriptHolder(std::string identifier_) : identifier(std::move(identifier_)) {
    auto* scriptEngine = AngelScriptVM::getScriptEngine();

    CScriptBuilder builder;
    int r;
    if (r = builder.StartNewModule(scriptEngine, this->identifier.c_str()); r < 0) {
        Logger::log(LogType::LOG_ERROR, "AngelScript", TRF("error.angelscript.unrecoverable_error", this->identifier));
        return;
    }
    auto scriptData = Resource::getResource<StringResource>(this->identifier);
    if (r = builder.AddSectionFromMemory(this->identifier.c_str(), scriptData->getString().c_str()); r < 0) {
        Logger::log(LogType::LOG_ERROR, "AngelScript", TRF("error.angelscript.script_not_found", this->identifier));
        return;
    }
    if (r = builder.BuildModule(); r < 0) {
        Logger::log(LogType::LOG_ERROR, "AngelScript", TRF("error.angelscript.compilation_failure", this->identifier));
        return;
    }

    this->context = scriptEngine->CreateContext();
    this->module = scriptEngine->GetModule(this->identifier.c_str());
}

AngelScriptHolder::~AngelScriptHolder() {
    this->context->Release();
}

std::function<void()> AngelScriptHolder::getFunction(std::string_view funcName) const {
    std::string funcNameFull{"void "};
    funcNameFull += funcName.data();
    funcNameFull += "()";

    if (auto* func = this->module->GetFunctionByDecl(funcNameFull.c_str()); func) {
        return [&, func] {
            this->context->Prepare(func);
            if (int r = this->context->Execute(); r != asEXECUTION_FINISHED) {
                Logger::log(LogType::LOG_ERROR, "AngelScript", TRF("error.angelscript.exception", this->identifier, this->context->GetExceptionString()));
            }
        };
    } else {
        Logger::log(LogType::LOG_ERROR, "AngelScript", TRF("error.angelscript.missing_function", this->identifier, funcNameFull));
        return [] {};
    }
}
