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

AngelscriptProvider::AngelscriptProvider() {
    this->started = false;
}

void AngelscriptProvider::initProvider() {
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

    this->registerGlobalFunction(AngelscriptProvider::print, "print");
}

void AngelscriptProvider::initScripts() {
    for (const auto& script : this->scripts) {
        script->init(this);
    }
    this->started = true;
}

void AngelscriptProvider::render(double delta) {
    for (const auto& script : this->scripts) {
        script->render(this, delta);
    }
}

void AngelscriptProvider::stop() {
    for (const auto& script : this->scripts) {
        script->stop(this);
    }
}

void AngelscriptProvider::addScript(const std::string& script) {
    this->scripts.push_back(std::make_unique<AngelscriptHolder>(script));
    if (this->started) {
        this->scripts[this->scripts.size() - 1]->init(this);
    }
}

void AngelscriptProvider::print(const std::string& message) {
    Logger::log(LogType::OUTPUT, "AngelScript", message);
}

void AngelscriptProvider::messageCallback(const asSMessageInfo* msg, void* param) {
    switch (msg->type) {
        case asMSGTYPE_INFORMATION:
            Logger::log(LogType::OUTPUT, "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
            break;
        case asMSGTYPE_WARNING:
            Logger::log(LogType::WARNING, "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
            break;
        case asMSGTYPE_ERROR:
            Logger::log(LogType::ERROR, "AngelScript",std::string(msg->section) + " (" + std::to_string(msg->row) + ", " + std::to_string(msg->col) + "): " + msg->message);
            break;
    }
}
