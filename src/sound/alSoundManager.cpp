#include "alSoundManager.h"
#include "oggFileStream.h"

alSoundManager::~alSoundManager() {
    if (!alcCall(alcMakeContextCurrent, this->contextCurrent, this->device, nullptr)) {
        engine::logError("OpenAL", "Could not make context non-current");
    }
    if (!alcCall(alcDestroyContext, this->device, this->context)) {
        engine::logError("OpenAL", "Could not destroy context");
    }
    ALCboolean closed;
    if (!alcCall(alcCloseDevice, closed, this->device, this->device)) {
        engine::logError("OpenAL", "Default device failed to close");
    }
}

void alSoundManager::init() {
    this->device = alcOpenDevice(nullptr);
    if (!this->device) {
        engine::logError("OpenAL", "Default device failed to initialize");
    }
    if (!alcCall(alcCreateContext, this->context, this->device, this->device, nullptr) || !this->context) {
        engine::logError("OpenAL", "Could not create context");
    }
    if (!alcCall(alcMakeContextCurrent, this->contextCurrent, this->device, this->context) || this->contextCurrent != ALC_TRUE) {
        engine::logError("OpenAL", "Could not make context current");
    }
}

void alSoundManager::update() {
    for (auto& [name, sound] : this->sounds) {
        sound.get()->update();
    }
}

void alSoundManager::stop() {
    for (auto& [name, sound] : this->sounds) {
        sound.get()->stop();
        sound.get()->discard();
    }
}

void alSoundManager::setListenerPosition(float x, float y, float z) {
    alListener3f(AL_POSITION, x, y, z);
}

void alSoundManager::setListenerRotation(float x, float y, float z) {
    float vec[6] = {x, y, z, 0.0f, 1.0f, 0.0f};
    alListenerfv(AL_ORIENTATION, vec);
}

void alSoundManager::addSound(const std::string& soundName, abstractSound* sound) {
    this->sounds.insert(std::pair(soundName, sound));
}

abstractSound* alSoundManager::getSound(const std::string& soundName) {
    return this->sounds.at(soundName).get();
}
