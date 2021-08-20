#include "angelscriptProvider.h"
#include <datetime/datetime.h>
#include <scriptany/scriptany.h>
#include <scriptarray/scriptarray.h>
#include <scriptdictionary/scriptdictionary.h>
#include <scripthandle/scripthandle.h>
#include <scriptmath/scriptmath.h>
#include <scriptmath/scriptmathcomplex.h>
#include <scriptstdstring/scriptstdstring.h>
#include <weakref/weakref.h>

angelscriptProvider::angelscriptProvider() {
    this->started = false;
}

void angelscriptProvider::initProvider() {
    this->asEngine = asCreateScriptEngine();
    int r;

    r = this->asEngine->SetMessageCallback(asFUNCTION(this->messageCallback), nullptr, asCALL_CDECL);
    assert(r >= 0);

    RegisterScriptDateTime(this->asEngine);
    RegisterStdString(this->asEngine);
    RegisterScriptArray(this->asEngine, true);
    RegisterScriptDictionary(this->asEngine);
    RegisterScriptMath(this->asEngine);
    RegisterScriptMathComplex(this->asEngine);
    RegisterScriptHandle(this->asEngine);
    RegisterScriptWeakRef(this->asEngine);
    RegisterScriptAny(this->asEngine);

    this->registerGlobalFunction(angelscriptProvider::print, "print");
}

void angelscriptProvider::initScripts() {
    for (const auto& script : this->scripts) {
        script->init(this);
    }
    this->started = true;
}

void angelscriptProvider::render(double delta) {
    for (const auto& script : this->scripts) {
        script->render(this, delta);
    }
}

void angelscriptProvider::stop() {
    for (const auto& script : this->scripts) {
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
    engine::logOutput("AngelScript", message);
}

void angelscriptProvider::messageCallback(const asSMessageInfo* msg, void* param) {
    if (msg->type == asMSGTYPE_INFORMATION) {
        engine::logOutput( "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
    } else if (msg->type == asMSGTYPE_WARNING) {
        engine::logWarning("AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
    } else if (msg->type == asMSGTYPE_ERROR) {
        engine::logError(  "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
    }
}
