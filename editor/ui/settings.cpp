#include "settings.h"

#include <misc/cpp/imgui_stdlib.h>
#include <resource/resource.h>
#include <core/engine.h>

#define SETTINGS_INPUT_FIELD_WIDTH 150.0f

using namespace chira;

settings::settings(bool startVisible, const ImVec2& windowSize, bool enforceSize) : abstractUiWindowComponent(TR("ui.settings.title"), startVisible, windowSize, enforceSize) {
    engine::getSettingsLoader()->getValue("graphics", "windowWidth", &this->windowWidth);
    engine::getSettingsLoader()->getValue("graphics", "windowHeight", &this->windowHeight);
    engine::getSettingsLoader()->getValue("graphics", "startMaximized", &this->startMaximized);
    engine::getSettingsLoader()->getValue("graphics", "fullscreen", &this->fullscreen);
    engine::getSettingsLoader()->getValue("input", "rawMouseMotion", &this->rawMouseMotion);
    engine::getSettingsLoader()->getValue("input", "invertYAxis", &this->invertYAxis);
    engine::getSettingsLoader()->getValue("ui", "language", &this->language);
    engine::getSettingsLoader()->getValue("engineGui", "discordIntegration", &this->discordIntegration);
}

void settings::draw(double delta) {
    ImGui::PushItemWidth(SETTINGS_INPUT_FIELD_WIDTH);
    ImGui::InputInt(TR("ui.settings.window_width").c_str(), &this->windowWidth);
    ImGui::InputInt(TR("ui.settings.window_height").c_str(), &this->windowHeight);
    ImGui::PopItemWidth();
    ImGui::Checkbox(TR("ui.settings.start_maximized").c_str(), &this->startMaximized);
    ImGui::Checkbox(TR("ui.settings.fullscreen").c_str(), &this->fullscreen);
    ImGui::Checkbox(TR("ui.settings.raw_mouse_motion").c_str(), &this->rawMouseMotion);
    ImGui::Checkbox(TR("ui.settings.invert_y_axis").c_str(), &this->invertYAxis);
    ImGui::PushItemWidth(SETTINGS_INPUT_FIELD_WIDTH);
    ImGui::InputText(TR("ui.settings.language").c_str(), &this->language);
    ImGui::PopItemWidth();
    ImGui::Checkbox(TR("ui.settings.discord_integration").c_str(), &this->discordIntegration);
    ImGui::Separator();
    if (ImGui::Button(TR("ui.settings.apply").c_str())) {
        engine::getSettingsLoader()->setValue("graphics", "windowWidth", this->windowWidth, true, false);
        engine::getSettingsLoader()->setValue("graphics", "windowHeight", this->windowHeight, true, false);
        engine::getSettingsLoader()->setValue("graphics", "startMaximized", this->startMaximized, true, false);
        engine::getSettingsLoader()->setValue("graphics", "fullscreen", this->fullscreen, true, false);
        engine::getSettingsLoader()->setValue("input", "rawMouseMotion", this->rawMouseMotion, true, false);
        engine::getSettingsLoader()->setValue("input", "invertYAxis", this->invertYAxis, true, false);
        engine::getSettingsLoader()->setValue("ui", "language", this->language, true, false);
        engine::getSettingsLoader()->setValue("engineGui", "discordIntegration", this->discordIntegration, true, false);
        engine::getSettingsLoader()->save();
    }
}
