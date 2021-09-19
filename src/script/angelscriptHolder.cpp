#include "angelscriptHolder.h"
#include "../resource/filesystemResourceProvider.h"
#include "../resource/resourceManager.h"
#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

// todo: use resource system to load scripts without a file path

angelscriptHolder::angelscriptHolder(const std::string& path) {
    this->filepath = ((filesystemResourceProvider*) resourceManager::getResourceProviderWithResource("file", "scripts/" + path))->getPath() + "/scripts/" + path;
}

angelscriptHolder::~angelscriptHolder() {
    this->scriptContext->Release();
}

void angelscriptHolder::init(angelscriptProvider* provider) {
    CScriptBuilder builder;
    int r;

    r = builder.StartNewModule(provider->asEngine, this->filepath.c_str());
    if (r < 0) {
        chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.unrecoverable_error"), this->filepath));
        return;
    }
    r = builder.AddSectionFromFile(this->filepath.c_str());
    if (r < 0) {
        chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.script_not_found"), this->filepath));
        return;
    }
    r = builder.BuildModule();
    if (r < 0) {
        chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.compilation_failure"), this->filepath));
        return;
    }

    this->scriptContext = provider->asEngine->CreateContext();
    asIScriptModule* module = provider->asEngine->GetModule(this->filepath.c_str());
    this->initFunc = module->GetFunctionByDecl("void init()");
    if (this->initFunc == nullptr) {
        chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.missing_function"), this->filepath, "void init()"));
        return;
    }
    this->renderFunc = module->GetFunctionByDecl("void render(double delta)");
    if (this->renderFunc == nullptr) {
        chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.missing_function"), this->filepath, "void render(double delta)"));
        return;
    }
    this->stopFunc = module->GetFunctionByDecl("void stop()");
    if (this->stopFunc == nullptr) {
        chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.missing_function"), this->filepath, "void stop()"));
        return;
    }

    this->scriptContext->Prepare(this->initFunc);
    r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.exception"), this->filepath, this->scriptContext->GetExceptionString()));
        }
    }
}

void angelscriptHolder::render(angelscriptProvider* provider, double delta) {
    this->scriptContext->Prepare(this->renderFunc);
    this->scriptContext->SetArgDouble(0, delta);
    int r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.exception"), this->filepath, this->scriptContext->GetExceptionString()));
        }
    }
}

void angelscriptHolder::stop(angelscriptProvider* provider) {
    this->scriptContext->Prepare(this->stopFunc);
    int r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            chira::logger::log(ERR, "AngelScript", fmt::format(TR("error.angelscript.exception"), this->filepath, this->scriptContext->GetExceptionString()));
        }
    }
}
