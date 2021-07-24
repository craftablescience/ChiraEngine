#pragma once

#include <string>
#include <map>
#include "abstractSound.h"

class abstractSoundManager {
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void stop() = 0;
    virtual void setListenerPosition(glm::vec3 newPosition) = 0;
    virtual void setListenerRotation(glm::vec3 newRotation, glm::vec3 up) = 0;
    virtual void addSound(const std::string& soundName, abstractSound* sound) = 0;
    virtual abstractSound* getSound(const std::string& soundName) = 0;
protected:
    std::map<std::string, std::unique_ptr<abstractSound>> sounds;
};
