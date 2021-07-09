#include "settings.h"

settings::settings(const std::string& filepath) {
    this->filepath = filepath;
    this->setWindowSize(640, 480);
    this->fullscreen = false;
}

void settings::save() {
    // todo: save settings
}

void settings::load() {
    // todo: load settings
    std::ofstream configFile(filepath);
    if (!configFile.is_open()) {
        std::cerr << "Error: settings file unreadable, loading defaults" << std::endl;
    } else {
        configFile << "hi";
        configFile.close();
    }
}

void settings::setWindowSize(int width, int height) {
    this->windowSize.first  = width;
    this->windowSize.second = height;
}

void settings::setFullscreen(bool enableFullscreen) {
    this->fullscreen = enableFullscreen;
}
