#include "AngelScriptVM.h"

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
#include <script/ASBindings.h>

#include <core/Logger.h>

using namespace chira;

// list of bindings
std::function<void()> liblist[] {
    ASB_ImGui
};


CHIRA_CREATE_LOG(ANGELSCRIPTVM);

static void messageCallback(const asSMessageInfo* msg, void*);
static void print(const std::string &message);

void AngelScriptVM::init() {
    AngelScriptVM::asEngine = asCreateScriptEngine();
    AngelScriptVM::asEngine->SetMessageCallback(asFUNCTION(messageCallback), nullptr, asCALL_CDECL);

    RegisterScriptDateTime(   AngelScriptVM::asEngine);
    RegisterStdString(        AngelScriptVM::asEngine);
    RegisterScriptArray(      AngelScriptVM::asEngine, true);
    RegisterStdStringUtils(   AngelScriptVM::asEngine);
    RegisterScriptDictionary( AngelScriptVM::asEngine);
    RegisterScriptMath(       AngelScriptVM::asEngine);
    RegisterScriptMathComplex(AngelScriptVM::asEngine);
    RegisterScriptGrid(       AngelScriptVM::asEngine);
    RegisterScriptHandle(     AngelScriptVM::asEngine);
    RegisterScriptWeakRef(    AngelScriptVM::asEngine);
    RegisterScriptAny(AngelScriptVM::asEngine);

    AngelScriptVM::registerGlobalFunction(print, "print");

    // register bindings
    for (std::function<void()> func : liblist)
    {
        func();
    }
    
}

static void messageCallback(const asSMessageInfo* msg, void*) {
    switch (msg->type) {
        case asMSGTYPE_INFORMATION:
            LOG_ANGELSCRIPTVM.output() <<
                    std::string(msg->section) + " (" +
                    std::to_string(msg->row) + ", " +
                    std::to_string(msg->col) + "): " + msg->message;
            break;
        case asMSGTYPE_WARNING:
            LOG_ANGELSCRIPTVM.warning() <<
                    std::string(msg->section) + " (" +
                    std::to_string(msg->row) + ", " +
                    std::to_string(msg->col) + "): " + msg->message;
            break;
        case asMSGTYPE_ERROR:
            LOG_ANGELSCRIPTVM.error() <<
                    std::string(msg->section) + " (" +
                    std::to_string(msg->row) + ", " +
                    std::to_string(msg->col) + "): " + msg->message;
            break;
    }
}

static void print(const std::string& message) {
    LOG_ANGELSCRIPTVM.output(message);
}
