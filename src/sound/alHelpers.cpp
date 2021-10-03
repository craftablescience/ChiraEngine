#include "alHelpers.h"

#include "../i18n/translationManager.h"
#include "fmt/core.h"

using namespace chira;

bool checkForALErrors(const std::string& filename, const std::uint_fast32_t line) {
    ALenum alError = alGetError();
    if (alError != AL_NO_ERROR) {
        std::string error;
        switch (alError) {
            case AL_INVALID_NAME:
                error = TR("error.openal.al_invalid_name");
                break;
            case AL_INVALID_ENUM:
                error = TR("error.openal.al_invalid_enum");
                break;
            case AL_INVALID_VALUE:
                error = TR("error.openal.al_invalid_value");
                break;
            case AL_INVALID_OPERATION:
                error = TR("error.openal.al_invalid_operation");
                break;
            case AL_OUT_OF_MEMORY:
                error = TR("error.openal.al_out_of_memory");
                break;
            default:
                error = fmt::format(TR("error.openal.al_unknown"), alError);
        }
        logger::log(ERR, "OpenAL", fmt::format(TR("error.openal.generic"), filename, line, error));
        return false;
    }
    return true;
}

bool checkForALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device) {
    ALCenum alcError = alcGetError(device);
    if (alcError != ALC_NO_ERROR) {
        std::string error;
        switch (alcError) {
            case ALC_INVALID_VALUE:
                error = TR("error.openal.alc_invalid_value");
                break;
            case ALC_INVALID_DEVICE:
                error = TR("error.openal.alc_invalid_device");
                break;
            case ALC_INVALID_CONTEXT:
                error = TR("error.openal.alc_invalid_context");
                break;
            case ALC_INVALID_ENUM:
                error = TR("error.openal.alc_invalid_enum");
                break;
            case ALC_OUT_OF_MEMORY:
                error = TR("error.openal.alc_out_of_memory");
                break;
            default:
                error = fmt::format(TR("error.openal.alc_unknown"), alcError);
        }
        logger::log(ERR, "OpenAL", fmt::format(TR("error.openal.generic"), filename, line, error));
        return false;
    }
    return true;
}

bool alcGetAvailableDevices(std::vector<std::string>& devicesVec, ALCdevice* device) {
    const ALCchar* devices;
    if(!alcCall(alcGetString, devices, device, nullptr, ALC_DEVICE_SPECIFIER))
        return false;
    const char* ptr = devices;
    devicesVec.clear();
    do {
        devicesVec.emplace_back(ptr);
        ptr += devicesVec.back().size() + 1;
    } while(*(ptr + 1) != '\0');
    return true;
}
