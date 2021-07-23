#ifndef BASICGAMEENGINE_ALHELPERS_H
#define BASICGAMEENGINE_ALHELPERS_H


#include <AL/al.h>
#include <AL/alc.h>
#include "../core/engine.h"

/*
 * This code is largely copied verbatim from:
 * https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/
 * The error reporting functions have been modified to use the engine's logging capabilities.
 * This code was in the public domain and was not under any licenses or restrictions.
 */

#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)

bool alCheckForErrors(const std::string& filename, const std::uint_fast32_t line) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::string out = "In " + filename + " at line " + line + ": ";
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
        engine::logError("OpenAL", out);
        return false;
    }
    return true;
}

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                const std::uint_fast32_t line,
                alFunction function,
                Params... params)
                ->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> {
    auto ret = function(std::forward<Params>(params)...);
    alCheckForErrors(filename, line);
    return ret;
}

template<typename alFunction, typename... Params>
auto alCallImpl(const char* filename,
                const std::uint_fast32_t line,
                alFunction function,
                Params... params)
                ->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
    function(std::forward<Params>(params)...);
    return alCheckForErrors(filename, line);
}

#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

bool alcCheckForErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device) {
    ALCenum error = alcGetError(device);
    if (error != ALC_NO_ERROR) {
        std::string out = "In " + filename + " at line " + line + ": ";
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
        engine::logError("OpenAL", out);
        return false;
    }
    return true;
}

template<typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename,
                 const std::uint_fast32_t line,
                 alcFunction function,
                 ALCdevice* device,
                 Params... params)
                 ->typename std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool> {
    function(std::forward<Params>(params)...);
    return alcCheckForErrors(filename, line, device);
}

template<typename alcFunction, typename ReturnType, typename... Params>
auto alcCallImpl(const char* filename,
                 const std::uint_fast32_t line,
                 alcFunction function,
                 ReturnType& returnValue,
                 ALCdevice* device,
                 Params... params)
                 ->typename std::enable_if_t<!std::is_same_v<void,decltype(function(params...))>,bool> {
    returnValue = function(std::forward<Params>(params)...);
    return alcCheckForErrors(filename, line, device);
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


#endif //BASICGAMEENGINE_ALHELPERS_H
