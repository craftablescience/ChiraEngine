#ifndef BASICGAMEENGINE_SETTINGS_H
#define BASICGAMEENGINE_SETTINGS_H


#include <iostream>
#include <fstream>
#include "../core/engine.h"
#include "nlohmann/json.hpp"

class settings {
public:
    explicit settings(const std::string& filepath = "settings.json");
    void save();
    void load();

    void setWindowSize(int width, int height);
    void setFullscreen(bool enableFullscreen);
private:
    std::string filepath;
    std::pair<int,int> windowSize;
    bool fullscreen;
};


#endif //BASICGAMEENGINE_SETTINGS_H
