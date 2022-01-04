#include "alSoundManager.h"

#include <i18n/translationManager.h>
#include <utility/logger.h>

using namespace chira;

ALSoundManager::~ALSoundManager() {
    if (!alcCall(alcMakeContextCurrent, this->contextCurrent, this->device, nullptr)) {
        Logger::log(LogType::ERROR, "OpenAL", TR("error.openal.remove_context"));
    }
    if (!alcCall(alcDestroyContext, this->device, this->context)) {
        Logger::log(LogType::ERROR, "OpenAL", TR("error.openal.destroy_context"));
    }
    ALCboolean closed;
    if (!alcCall(alcCloseDevice, closed, this->device, this->device)) {
        Logger::log(LogType::ERROR, "OpenAL", TR("error.openal.close_device_failure"));
    }
}

void ALSoundManager::init() {
    this->device = alcOpenDevice(nullptr);
    if (!this->device) {
        Logger::log(LogType::WARNING, "OpenAL", TR("error.openal.open_device_failure"));
        std::vector<std::string> devices{};
        alcGetAvailableDevices(devices, nullptr);
        if (devices.empty()) {
            Logger::log(LogType::ERROR, "OpenAL", TR("error.openal.no_devices_available"));
        } else {
            Logger::log(LogType::ERROR, "OpenAL", fmt::format(TR("error.openal.using_nondefault_device"), devices[0]));
            this->device = alcOpenDevice(devices[0].c_str());
        }
    }
    if (!alcCall(alcCreateContext, this->context, this->device, this->device, nullptr) || !this->context) {
        Logger::log(LogType::ERROR, "OpenAL", TR("error.openal.create_context_failure"));
    }
    if (!alcCall(alcMakeContextCurrent, this->contextCurrent, this->device, this->context) || this->contextCurrent != ALC_TRUE) {
        Logger::log(LogType::ERROR, "OpenAL", TR("error.openal.make_context_current"));
    }
}

void ALSoundManager::update() {
    if (this->context) {
        for (const auto& sound : this->sounds) {
            sound.second->update();
        }
    }
}

void ALSoundManager::stop() {
    for (const auto& sound : this->sounds) {
        sound.second->stop();
        sound.second->discard();
    }
}

void ALSoundManager::setListenerPosition(const glm::vec3& newPosition) {
    if (this->context) {
        alListener3f(AL_POSITION, newPosition.x, newPosition.y, newPosition.z);
    }
}

void ALSoundManager::setListenerRotation(const glm::vec3& newRotation, const glm::vec3& up) {
    if (this->context) {
        float vec[6] = {newRotation.x, newRotation.y, newRotation.z, up.x, up.y, up.z};
        alListenerfv(AL_ORIENTATION, vec);
    }
}

void ALSoundManager::addSound(const std::string& soundName, AbstractSound* sound) {
    this->sounds.insert(std::pair(soundName, sound));
}

AbstractSound* ALSoundManager::getSound(const std::string& soundName) const {
    return this->sounds.at(soundName).get();
}
