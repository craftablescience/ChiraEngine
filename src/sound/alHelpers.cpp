#include "alHelpers.h"

bool checkForALErrors(const std::string& filename, const std::uint_fast32_t line) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::string out = "In " + filename + " at line " + std::to_string(line) + ": ";
        switch (error) {
            case AL_INVALID_NAME:
                out += "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                out += "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                out += "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                out += "AL_INVALID_OPERATION: the requested operation is not valid";
                break;
            case AL_OUT_OF_MEMORY:
                out += "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                break;
            default:
                out += "UNKNOWN AL ERROR: " + std::to_string(error);
        }
        chira::logger::log(ERR, "OpenAL", out);
        return false;
    }
    return true;
}

bool checkForALCErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device) {
    ALCenum error = alcGetError(device);
    if (error != ALC_NO_ERROR) {
        std::string out = "In " + filename + " at line " + std::to_string(line) + ": ";
        switch (error) {
            case ALC_INVALID_VALUE:
                out += "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case ALC_INVALID_DEVICE:
                out += "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
                break;
            case ALC_INVALID_CONTEXT:
                out += "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
                break;
            case ALC_INVALID_ENUM:
                out += "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
                break;
            case ALC_OUT_OF_MEMORY:
                out += "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function";
                break;
            default:
                out += "UNKNOWN ALC ERROR: " + std::to_string(error);
        }
        chira::logger::log(ERR, "OpenAL", out);
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
