#include "alSoundManager.h"

#include "fmt/core.h"
#include "../i18n/translationManager.h"

using namespace chira;

alSoundManager::~alSoundManager() {
    if (!alcCall(alcMakeContextCurrent, this->contextCurrent, this->device, nullptr)) {
        logger::log(ERR, "OpenAL", TR("error.openal.remove_context"));
    }
    if (!alcCall(alcDestroyContext, this->device, this->context)) {
        logger::log(ERR, "OpenAL", TR("error.openal.destroy_context"));
    }
    ALCboolean closed;
    if (!alcCall(alcCloseDevice, closed, this->device, this->device)) {
        logger::log(ERR, "OpenAL", TR("error.openal.close_device_failure"));
    }
}

void alSoundManager::init() {
    this->device = alcOpenDevice(nullptr);
    if (!this->device) {
        logger::log(WARN, "OpenAL", TR("error.openal.open_device_failure"));
        std::vector<std::string> devices{};
        alcGetAvailableDevices(devices, nullptr);
        if (devices.empty()) {
            logger::log(ERR, "OpenAL", TR("error.openal.no_devices_available"));
        } else {
            logger::log(ERR, "OpenAL", fmt::format(TR("error.openal.using_nondefault_device"), devices[0]));
            this->device = alcOpenDevice(devices[0].c_str());
        }
    }
    if (!alcCall(alcCreateContext, this->context, this->device, this->device, nullptr) || !this->context) {
        logger::log(ERR, "OpenAL", TR("error.openal.create_context_failure"));
    }
    if (!alcCall(alcMakeContextCurrent, this->contextCurrent, this->device, this->context) || this->contextCurrent != ALC_TRUE) {
        logger::log(ERR, "OpenAL", TR("error.openal.make_context_current"));
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
