#include "AngelScriptHolder.h"

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

#include <resource/StringResource.h>

using namespace chira;

CHIRA_CREATE_LOG(ANGELSCRIPT);

AngelScriptHolder::AngelScriptHolder(std::string identifier_) : identifier(std::move(identifier_)) {
    auto* scriptEngine = AngelScriptVM::getScriptEngine();

    CScriptBuilder builder;
    int r;
    if (r = builder.StartNewModule(scriptEngine, this->identifier.c_str()); r < 0) {
        LOG_ANGELSCRIPT.error("Unrecoverable error while starting a new module at \"{}\"", this->identifier);
        return;
    }
    auto scriptData = Resource::getResource<StringResource>(this->identifier);
    if (r = builder.AddSectionFromMemory(this->identifier.c_str(), scriptData->getString().c_str()); r < 0) {
        LOG_ANGELSCRIPT.error("Unable to load script at \"{}\"", this->identifier);
        return;
    }
    if (r = builder.BuildModule(); r < 0) {
        LOG_ANGELSCRIPT.error("Script errors in \"{}\" have forced compilation to fail!", this->identifier);
        return;
    }

    this->context = scriptEngine->CreateContext();
    this->module = scriptEngine->GetModule(this->identifier.c_str());
}

AngelScriptHolder::~AngelScriptHolder() {
    this->context->Release();
}
