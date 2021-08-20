#pragma once

#include <glm/glm.hpp>
#include "alHelpers.h"
#include "abstractSoundManager.h"
#include "abstractSound.h"

class alSoundManager : public abstractSoundManager {
public:
    virtual ~alSoundManager();
    void init() override;
    void update() override;
    void stop() override;
    void setListenerPosition(const glm::vec3& newPosition) override;
    void setListenerRotation(const glm::vec3& newRotation, const glm::vec3& up) override;
    void addSound(const std::string& soundName, abstractSound* sound) override;
    abstractSound* getSound(const std::string& soundName) override;
private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    ALCboolean contextCurrent = AL_FALSE;
};
