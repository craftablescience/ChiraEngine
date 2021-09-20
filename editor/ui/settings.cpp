#include "settings.h"

#include "misc/cpp/imgui_stdlib.h"
#include "../../src/resource/resourceManager.h"
#include "../../src/core/engine.h"

#define SETTINGS_INPUT_FIELD_WIDTH 150.0f

settings::settings() {
    this->isEnabled = false;
    engine::getSettingsLoader()->getValue("graphics", "windowWidth", &this->windowWidth);
    engine::getSettingsLoader()->getValue("graphics", "windowHeight", &this->windowHeight);
    engine::getSettingsLoader()->getValue("graphics", "startMaximized", &this->startMaximized);
    engine::getSettingsLoader()->getValue("graphics", "fullscreen", &this->fullscreen);
    engine::getSettingsLoader()->getValue("input", "rawMouseMotion", &this->rawMouseMotion);
    engine::getSettingsLoader()->getValue("input", "invertYAxis", &this->invertYAxis);
    engine::getSettingsLoader()->getValue("ui", "language", &this->language);
    engine::getSettingsLoader()->getValue("engineGui", "discordIntegration", &this->discordIntegration);
}

void settings::render() {
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(TR("ui.settings.title").c_str(), &(this->isEnabled))) {
        ImGui::End();
        return;
    }

    ImGui::PushItemWidth(SETTINGS_INPUT_FIELD_WIDTH);
    ImGui::InputInt("ui.settings.window_width", &this->windowWidth);
    ImGui::InputInt("ui.settings.window_height", &this->windowHeight);
    ImGui::PopItemWidth();
    ImGui::Checkbox("ui.settings.start_maximized", &this->startMaximized);
    ImGui::Checkbox("ui.settings.fullscreen", &this->fullscreen);
    ImGui::Checkbox("ui.settings.raw_mouse_motion", &this->rawMouseMotion);
    ImGui::Checkbox("ui.settings.invert_y_axis", &this->invertYAxis);
    ImGui::PushItemWidth(SETTINGS_INPUT_FIELD_WIDTH);
    ImGui::InputText("ui.settings.language", &this->language);
    ImGui::PopItemWidth();
    ImGui::Checkbox("ui.settings.discord_integration", &this->discordIntegration);

    if (ImGui::Button("ui.settings.apply")) {
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

    ImGui::End();
}

void settings::setEnabled(bool enabled) {
    this->isEnabled = enabled;
}

bool settings::getEnabled() const {
    return this->isEnabled;
}
