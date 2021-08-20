#include "angelscriptHolder.h"
#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

angelscriptHolder::angelscriptHolder(const std::string& path) {
    this->filepath = virtualFileSystem::getScriptPath(path);
}

angelscriptHolder::~angelscriptHolder() {
    this->scriptContext->Release();
}

void angelscriptHolder::init(angelscriptProvider* provider) {
    CScriptBuilder builder;
    int r;

    r = builder.StartNewModule(provider->asEngine, this->filepath.c_str());
    if (r < 0) {
        chiraLogger::log(ERR, "AngelScript", std::string("Unrecoverable error while starting a new module at \"") + this->filepath + "\"!");
        return;
    }
    r = builder.AddSectionFromFile(this->filepath.c_str());
    if (r < 0) {
        chiraLogger::log(ERR, "AngelScript", std::string("Unable to load script at \"") + this->filepath + "\"!");
        return;
    }
    r = builder.BuildModule();
    if (r < 0) {
        chiraLogger::log(ERR, "AngelScript", std::string("Script errors in \"") + this->filepath + "\" have forced compilation to fail!");
        return;
    }

    this->scriptContext = provider->asEngine->CreateContext();
    asIScriptModule* module = provider->asEngine->GetModule(this->filepath.c_str());
    this->initFunc = module->GetFunctionByDecl("void init()");
    if (this->initFunc == nullptr) {
        chiraLogger::log(ERR, "AngelScript", std::string("Script at \"") + this->filepath + "\" does not have a \"void init()\" function!");
        return;
    }
    this->renderFunc = module->GetFunctionByDecl("void render(double delta)");
    if (this->renderFunc == nullptr) {
        chiraLogger::log(ERR, "AngelScript", std::string("Script at \"") + this->filepath + "\" does not have a \"void render(double delta)\" function!");
        return;
    }
    this->stopFunc = module->GetFunctionByDecl("void stop()");
    if (this->stopFunc == nullptr) {
        chiraLogger::log(ERR, "AngelScript", std::string("Script at \"") + this->filepath + "\" does not have a \"void stop()\" function!");
        return;
    }

    this->scriptContext->Prepare(this->initFunc);
    r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            chiraLogger::log(ERR, "AngelScript", std::string("An exception in \"") + this->filepath + "\" at INIT occurred: " + this->scriptContext->GetExceptionString());
        }
    }
}

void angelscriptHolder::render(angelscriptProvider* provider, double delta) {
    this->scriptContext->Prepare(this->renderFunc);
    this->scriptContext->SetArgDouble(0, delta);
    int r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            chiraLogger::log(ERR, "AngelScript", std::string("An exception in \"") + this->filepath + "\" occurred: " + this->scriptContext->GetExceptionString());
        }
    }
}

void angelscriptHolder::stop(angelscriptProvider* provider) {
    this->scriptContext->Prepare(this->stopFunc);
    int r = this->scriptContext->Execute();
    if (r != asEXECUTION_FINISHED) {
        if (r == asEXECUTION_EXCEPTION) {
            chiraLogger::log(ERR, "AngelScript", std::string("An exception in \"") + this->filepath + "\" occurred: " + this->scriptContext->GetExceptionString());
        }
    }
}
