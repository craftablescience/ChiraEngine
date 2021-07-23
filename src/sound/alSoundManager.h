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
    void setListenerPosition(float x, float y, float z) override;
    void setListenerRotation(float x, float y, float z) override;
    void addSound(const std::string& soundName, abstractSound* sound) override;
    abstractSound* getSound(const std::string& soundName) override;
private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    ALCboolean contextCurrent = AL_FALSE;
};
