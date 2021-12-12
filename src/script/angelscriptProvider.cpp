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

#include <utility/logger.h>

using namespace chira;

angelscriptProvider::angelscriptProvider() {
    this->started = false;
}

void angelscriptProvider::initProvider() {
    this->asEngine = asCreateScriptEngine();
    this->asEngine->SetMessageCallback(asFUNCTION(this->messageCallback), nullptr, asCALL_CDECL);

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

void angelscriptProvider::addScript(const std::string& script) {
    this->scripts.push_back(std::make_unique<angelscriptHolder>(script));
    if (this->started) {
        this->scripts[this->scripts.size() - 1]->init(this);
    }
}

void angelscriptProvider::print(const std::string& message) {
    logger::log(OUTPUT, "AngelScript", message);
}

void angelscriptProvider::messageCallback(const asSMessageInfo* msg, void* param) {
    switch (msg->type) {
        case asMSGTYPE_INFORMATION:
            logger::log(OUTPUT, "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
            break;
        case asMSGTYPE_WARNING:
            logger::log(WARN, "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
            break;
        case asMSGTYPE_ERROR:
            logger::log(ERR,   "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
            break;
    }
}
