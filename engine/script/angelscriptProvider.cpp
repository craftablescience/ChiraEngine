#include "angelscriptProvider.h"

#include <datetime/datetime.h>
#include <scriptany/scriptany.h>
#include <scriptarray/scriptarray.h>
#include <scriptdictionary/scriptdictionary.h>
#include <scriptgrid/scriptgrid.h>
#include <scripthandle/scripthandle.h>
#include <scriptmath/scriptmath.h>
#include <scriptmath/scriptmathcomplex.h>
#include <scriptstdstring/scriptstdstring.h>
#include <weakref/weakref.h>
#include <utility/platform.h>
#include <utility/logger.h>

using namespace chira;

static void CHIRA_CDECL messageCallback(const asSMessageInfo* msg, void*) {
    switch (msg->type) {
        case asMSGTYPE_INFORMATION:
            Logger::log(LogType::OUTPUT, "AngelScript",
                        std::string(msg->section) + " (" +
                        std::to_string(msg->row) + ", " +
                        std::to_string(msg->col) + "): " + msg->message);
            break;
        case asMSGTYPE_WARNING:
            Logger::log(LogType::WARNING, "AngelScript",
                        std::string(msg->section) + " (" +
                        std::to_string(msg->row) + ", " +
                        std::to_string(msg->col) + "): " + msg->message);
            break;
        case asMSGTYPE_ERROR:
            Logger::log(LogType::ERROR, "AngelScript",
                        std::string(msg->section) + " (" +
                        std::to_string(msg->row) + ", " +
                        std::to_string(msg->col) + "): " + msg->message);
            break;
    }
}

static void CHIRA_CDECL print(const std::string& message) {
    Logger::log(LogType::OUTPUT, "AngelScript", message);
}

AngelscriptProvider::AngelscriptProvider() {
    this->asEngine = asCreateScriptEngine();
    this->asEngine->SetMessageCallback(asFUNCTION(messageCallback), nullptr, asCALL_CDECL);

    RegisterScriptDateTime(   this->asEngine);
    RegisterStdString(        this->asEngine);
    RegisterScriptArray(      this->asEngine, true);
    RegisterStdStringUtils(   this->asEngine);
    RegisterScriptDictionary( this->asEngine);
    RegisterScriptMath(       this->asEngine);
    RegisterScriptMathComplex(this->asEngine);
    RegisterScriptGrid(       this->asEngine);
    RegisterScriptHandle(     this->asEngine);
    RegisterScriptWeakRef(    this->asEngine);
    RegisterScriptAny(        this->asEngine);

    this->registerGlobalFunction(print, "print");
}

asIScriptEngine* AngelscriptProvider::getAngelscriptEngine() const {
    return this->asEngine;
}

void AngelscriptProvider::init() {
    for (const auto& script : this->scripts) {
        script->init(this);
    }
    this->started = true;
}

void AngelscriptProvider::render(double delta) {
    for (const auto& script : this->scripts) {
        script->render();
    }
}

void AngelscriptProvider::stop() {
    for (const auto& script : this->scripts) {
        script->stop();
    }
}

void AngelscriptProvider::addScript(const std::string& script) {
    this->scripts.push_back(std::make_unique<AngelscriptHolder>(script));
    if (this->started) {
        this->scripts[this->scripts.size() - 1]->init(this);
    }
}
