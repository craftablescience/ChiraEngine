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

void alSoundManager::setListenerPosition(glm::vec3 newPosition) {
    alListener3f(AL_POSITION, newPosition.x, newPosition.y, newPosition.z);
}

void alSoundManager::setListenerRotation(glm::vec3 newRotation, glm::vec3 up) {
    float vec[6] = {newRotation.x, newRotation.y, newRotation.z, up.x, up.y, up.z};
    alListenerfv(AL_ORIENTATION, vec);
}

void alSoundManager::addSound(const std::string& soundName, abstractSound* sound) {
    this->sounds.insert(std::pair(soundName, sound));
}

abstractSound* alSoundManager::getSound(const std::string& soundName) {
    return this->sounds.at(soundName).get();
}
