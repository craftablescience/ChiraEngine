/*
    AngelScript ImGui Bindings

    These are manually written as ImGui's special types
    can't be converted properly
*/

#include <script/AngelScriptVM.h>
#include <script/ASBindings.h>
#include <imgui.h>

using namespace chira;

static bool imBegin(const std::string& name, bool *is_open) {
    return ImGui::Begin(name.c_str(), is_open);
}

static void imText(const std::string& label) {
    ImGui::Text(label.c_str());
}

static void imEnd() {
    ImGui::End();
}

static bool imButton(const std::string& label) {
    return ImGui::Button(label.c_str());
}

void chira::ASB_ImGui() {
    const char *on = AngelScriptVM::getScriptEngine()->GetDefaultNamespace();
    
    AngelScriptVM::getScriptEngine()->SetDefaultNamespace("ImGui");

    AngelScriptVM::registerGlobalFunction(imBegin, "Begin");
    AngelScriptVM::registerGlobalFunction(imEnd, "End");
    AngelScriptVM::registerGlobalFunction(imText, "Text");
    AngelScriptVM::registerGlobalFunction(imButton, "Button");

    AngelScriptVM::getScriptEngine()->SetDefaultNamespace(on);
}