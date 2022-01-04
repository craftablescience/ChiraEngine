#include "settings.h"

#include <misc/cpp/imgui_stdlib.h>
#include <core/engine.h>
#include <i18n/translationManager.h>

using namespace chira;

Settings::Settings(const ImVec2& windowSize) : Window(TR("ui.settings.title"), false, windowSize) {
    Engine::getSettingsLoader()->getValue("graphics", "windowWidth", &this->windowWidth);
    Engine::getSettingsLoader()->getValue("graphics", "windowHeight", &this->windowHeight);
    Engine::getSettingsLoader()->getValue("graphics", "startMaximized", &this->startMaximized);
    Engine::getSettingsLoader()->getValue("graphics", "fullscreen", &this->fullscreen);
    Engine::getSettingsLoader()->getValue("input", "rawMouseMotion", &this->rawMouseMotion);
    Engine::getSettingsLoader()->getValue("input", "invertYAxis", &this->invertYAxis);
    Engine::getSettingsLoader()->getValue("ui", "language", &this->language);
    Engine::getSettingsLoader()->getValue("engineGui", "discordIntegration", &this->discordIntegration);
}

void Settings::renderContents() {
    constexpr float inputWidth = 150.0f;

    ImGui::PushItemWidth(inputWidth);
    ImGui::InputInt(TR("ui.settings.window_width").c_str(), &this->windowWidth);
    ImGui::InputInt(TR("ui.settings.window_height").c_str(), &this->windowHeight);
    ImGui::PopItemWidth();
    ImGui::Checkbox(TR("ui.settings.start_maximized").c_str(), &this->startMaximized);
    ImGui::Checkbox(TR("ui.settings.fullscreen").c_str(), &this->fullscreen);
    ImGui::Checkbox(TR("ui.settings.raw_mouse_motion").c_str(), &this->rawMouseMotion);
    ImGui::Checkbox(TR("ui.settings.invert_y_axis").c_str(), &this->invertYAxis);
    ImGui::PushItemWidth(inputWidth);
    ImGui::InputText(TR("ui.settings.language").c_str(), &this->language);
    ImGui::PopItemWidth();
    ImGui::Checkbox(TR("ui.settings.discord_integration").c_str(), &this->discordIntegration);
    ImGui::Separator();
    if (ImGui::Button(TR("ui.settings.apply").c_str())) {
        Engine::getSettingsLoader()->setValue("graphics", "windowWidth", this->windowWidth, true, false);
        Engine::getSettingsLoader()->setValue("graphics", "windowHeight", this->windowHeight, true, false);
        Engine::getSettingsLoader()->setValue("graphics", "startMaximized", this->startMaximized, true, false);
        Engine::getSettingsLoader()->setValue("graphics", "fullscreen", this->fullscreen, true, false);
        Engine::getSettingsLoader()->setValue("input", "rawMouseMotion", this->rawMouseMotion, true, false);
        Engine::getSettingsLoader()->setValue("input", "invertYAxis", this->invertYAxis, true, false);
        Engine::getSettingsLoader()->setValue("ui", "language", this->language, true, false);
        Engine::getSettingsLoader()->setValue("engineGui", "discordIntegration", this->discordIntegration, true, false);
        Engine::getSettingsLoader()->save();
    }
}
