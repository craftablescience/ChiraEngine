#include "angelscriptHolder.h"
#include "../resource/filesystemResourceProvider.h"
#include "../resource/resourceManager.h"
#include "../resource/stringResource.h"
#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

using namespace chira;

angelscriptHolder::angelscriptHolder(const std::string& identifier_) {
    this->identifier = identifier_;
}

angelscriptHolder::~angelscriptHolder() {
    this->scriptContext->Release();
}

void angelscriptHolder::init(angelscriptProvider* provider) {
    CScriptBuilder builder;
    int r;

    r = builder.StartNewModule(provider->asEngine, this->identifier.c_str());
    if (r < 0) {
        logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.unrecoverable_error"), this->identifier));
        return;
    }
    std::unique_ptr<stringResource> scriptData = resourceManager::getResourceWithoutCaching<stringResource>(this->identifier);
    r = builder.AddSectionFromMemory(this->identifier.c_str(), scriptData->getString().c_str());
    if (r < 0) {
        logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.script_not_found"), this->identifier));
        return;
    }
    r = builder.BuildModule();
    if (r < 0) {
        logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.compilation_failure"), this->identifier));
        return;
    }

    this->scriptContext = provider->asEngine->CreateContext();
    asIScriptModule* module = provider->asEngine->GetModule(this->identifier.c_str());
    this->initFunc = module->GetFunctionByDecl("void init()");
    if (this->initFunc == nullptr) {
        logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.missing_function"), this->identifier, "void init()"));
        return;
    }
    this->renderFunc = module->GetFunctionByDecl("void render(double delta)");
    if (this->renderFunc == nullptr) {
        logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.missing_function"), this->identifier, "void render(double delta)"));
        return;
    }
    this->stopFunc = module->GetFunctionByDecl("void stop()");
    if (this->stopFunc == nullptr) {
        logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.missing_function"), this->identifier, "void stop()"));
        return;
    }

    this->scriptContext->Prepare(this->initFunc);
    r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.exception"), this->identifier, this->scriptContext->GetExceptionString()));
        }
    }
}

void angelscriptHolder::render(angelscriptProvider* provider, double delta) {
    this->scriptContext->Prepare(this->renderFunc);
    this->scriptContext->SetArgDouble(0, delta);
    int r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.exception"), this->identifier, this->scriptContext->GetExceptionString()));
        }
    }
}

void angelscriptHolder::stop(angelscriptProvider* provider) {
    this->scriptContext->Prepare(this->stopFunc);
    int r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.exception"), this->identifier, this->scriptContext->GetExceptionString()));
        }
    }
}
