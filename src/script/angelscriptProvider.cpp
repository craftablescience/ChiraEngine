#include "angelscriptProvider.h"
#include <datetime/datetime.h>
#include <scriptany/scriptany.h>
#include <scriptarray/scriptarray.h>
#include <scriptmath/scriptmath.h>
#include <scriptmath/scriptmathcomplex.h>
#include <scriptstdstring/scriptstdstring.h>

angelscriptProvider::angelscriptProvider(engine* engine) {
    if (!angelscriptProvider::gameEngine) {
        angelscriptProvider::gameEngine = engine;
    }
    this->started = false;
}

void angelscriptProvider::initProvider() {
    this->asEngine = asCreateScriptEngine();
    int r;

    r = this->asEngine->SetMessageCallback(asFUNCTION(this->messageCallback), nullptr, asCALL_CDECL);
    assert(r >= 0);

    RegisterScriptDateTime(this->asEngine);
    RegisterScriptAny(this->asEngine);
    RegisterScriptArray(this->asEngine, true);
    RegisterScriptMath(this->asEngine);
    RegisterScriptMathComplex(this->asEngine);
    RegisterStdString(this->asEngine);

    this->registerGlobalFunction(angelscriptProvider::print, "print");
}

void angelscriptProvider::initScripts() {
    for (auto const& script : this->scripts) {
        script->init(this);
    }
    this->started = true;
}

void angelscriptProvider::render(double delta) {
    for (auto const& script : this->scripts) {
        script->render(this, delta);
    }
}

void angelscriptProvider::stop() {
    for (auto const& script : this->scripts) {
        script->stop(this);
    }
}

void angelscriptProvider::addScript(angelscriptHolder* script) {
    if (this->started) {
        script->init(this);
    }
    this->scripts.push_back(std::unique_ptr<angelscriptHolder>(script));
}

void angelscriptProvider::print(const std::string& message) {
    angelscriptProvider::gameEngine->logOutput("AngelScript", message);
}

void angelscriptProvider::messageCallback(const asSMessageInfo* msg, void* param) {
    if (msg->type == asMSGTYPE_INFORMATION) {
        angelscriptProvider::gameEngine->logOutput( "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
    } else if (msg->type == asMSGTYPE_WARNING) {
        angelscriptProvider::gameEngine->logWarning("AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
    } else if (msg->type == asMSGTYPE_ERROR) {
        angelscriptProvider::gameEngine->logError(  "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
    }
}
