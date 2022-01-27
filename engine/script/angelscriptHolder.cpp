#include "angelscriptHolder.h"

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <resource/stringResource.h>
#include <i18n/translationManager.h>
#include <utility/logger.h>

using namespace chira;

AngelscriptHolder::AngelscriptHolder(const std::string& identifier_) {
    this->identifier = identifier_;
}

AngelscriptHolder::~AngelscriptHolder() {
    this->scriptContext->Release();
}

void AngelscriptHolder::init(AngelscriptProvider* provider) {
    CScriptBuilder builder;
    int r;
    if (r = builder.StartNewModule(provider->getAngelscriptEngine(), this->identifier.c_str()); r < 0) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.unrecoverable_error", this->identifier));
        return;
    }
    auto scriptData = Resource::getResource<StringResource>(this->identifier);
    if (r = builder.AddSectionFromMemory(this->identifier.c_str(), scriptData->getString().c_str()); r < 0) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.script_not_found", this->identifier));
        return;
    }
    if (r = builder.BuildModule(); r < 0) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.compilation_failure", this->identifier));
        return;
    }

    this->scriptContext = provider->getAngelscriptEngine()->CreateContext();
    asIScriptModule* module = provider->getAngelscriptEngine()->GetModule(this->identifier.c_str());
    if (this->initFunc = module->GetFunctionByDecl("void init()"); !this->initFunc) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.missing_function", this->identifier, "void init()"));
        return;
    }
    if (this->renderFunc = module->GetFunctionByDecl("void render()"); !this->renderFunc) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.missing_function", this->identifier, "void render()"));
        return;
    }
    if (this->stopFunc = module->GetFunctionByDecl("void stop()"); !this->stopFunc) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.missing_function", this->identifier, "void stop()"));
        return;
    }

    this->scriptContext->Prepare(this->initFunc);
    if (r = this->scriptContext->Execute(); r != asEXECUTION_FINISHED) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.exception", this->identifier, this->scriptContext->GetExceptionString()));
    }
}

void AngelscriptHolder::render() {
    this->scriptContext->Prepare(this->renderFunc);
    if (int r = this->scriptContext->Execute(); r != asEXECUTION_FINISHED) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.exception", this->identifier, this->scriptContext->GetExceptionString()));
    }
}

void AngelscriptHolder::stop() {
    this->scriptContext->Prepare(this->stopFunc);
    if (int r = this->scriptContext->Execute(); r != asEXECUTION_FINISHED) {
        Logger::log(LogType::ERROR, "AngelScript", TRF("error.angelscript.exception", this->identifier, this->scriptContext->GetExceptionString()));
    }
}
