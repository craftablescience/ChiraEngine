#pragma once

#ifdef CHIRA_BUILD_WITH_MSVC_COMPAT
#define AL_LIBTYPE_STATIC
#endif
#include <AL/al.h>
#include <AL/alc.h>
#include <core/engine.h>

/*
 * This code is largely copied verbatim from:
 * https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/
 * The error reporting functions have been modified to use the engine's logging capabilities.
 * This code was in the public domain and was not under any licenses or restrictions.
 */

namespace chira {
    bool checkForALErrors(const std::string& filename, std::uint_fast32_t line);

    template<typename alFunction, typename... Params>
    auto alCallImpl(const char* filename,
                    const std::uint_fast32_t line,
                    alFunction function,
                    Params... params)
                    ->typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))> {
        auto ret = function(std::forward<Params>(params)...);
        checkForALErrors(filename, line);
        return ret;
    }

    template<typename alFunction, typename... Params>
    auto alCallImpl(const char* filename,
                    const std::uint_fast32_t line,
                    alFunction function,
                    Params... params)
                    ->typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool> {
        function(std::forward<Params>(params)...);
        return checkForALErrors(filename, line);
    }

    bool checkForALCErrors(const std::string& filename, std::uint_fast32_t line, ALCdevice* device);

    template<typename alcFunction, typename... Params>
    auto alcCallImpl(const char* filename,
                     const std::uint_fast32_t line,
                     alcFunction function,
                     ALCdevice* device,
                     Params... params)
                     ->typename std::enable_if_t<std::is_same_v<void,decltype(function(params...))>,bool> {
        function(std::forward<Params>(params)...);
        return checkForALCErrors(filename, line, device);
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
        return checkForALCErrors(filename, line, device);
    }

    bool alcGetAvailableDevices(std::vector<std::string>& devicesVec, ALCdevice* device);
}

#define alCall(function, ...) chira::alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) chira::alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)
