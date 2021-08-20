#include "alSoundManager.h"

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
        engine::logWarning("OpenAL", "Default device failed to initialize");
        std::vector<std::string> devices{};
        alcGetAvailableDevices(devices, nullptr);
        if (devices.empty()) {
            engine::logError("OpenAL", "No devices available!");
        } else {
            engine::logWarning("OpenAL", "Using non-default device " + devices[0]);
            this->device = alcOpenDevice(devices[0].c_str());
        }
    }
    if (!alcCall(alcCreateContext, this->context, this->device, this->device, nullptr) || !this->context) {
        engine::logError("OpenAL", "Could not create context");
    }
    if (!alcCall(alcMakeContextCurrent, this->contextCurrent, this->device, this->context) || this->contextCurrent != ALC_TRUE) {
        engine::logError("OpenAL", "Could not make context current");
    }
}

void alSoundManager::update() {
    if (this->context) {
        for (const auto& sound : this->sounds) {
            sound.second->update();
        }
    }
}

void alSoundManager::stop() {
    for (const auto& sound : this->sounds) {
        sound.second->stop();
        sound.second->discard();
    }
}

void alSoundManager::setListenerPosition(const glm::vec3& newPosition) {
    if (this->context) {
        alListener3f(AL_POSITION, newPosition.x, newPosition.y, newPosition.z);
    }
}

void alSoundManager::setListenerRotation(const glm::vec3& newRotation, const glm::vec3& up) {
    if (this->context) {
        float vec[6] = {newRotation.x, newRotation.y, newRotation.z, up.x, up.y, up.z};
        alListenerfv(AL_ORIENTATION, vec);
    }
}

void alSoundManager::addSound(const std::string& soundName, abstractSound* sound) {
    this->sounds.insert(std::pair(soundName, sound));
}

abstractSound* alSoundManager::getSound(const std::string& soundName) {
    return this->sounds.at(soundName).get();
}
