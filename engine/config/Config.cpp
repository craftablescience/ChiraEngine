#include "Config.h"

#ifdef DEBUG
    #include <filesystem>
#endif

#include <core/Platform.h>
#ifdef CHIRA_PLATFORM_WINDOWS
    #define _CRT_SECURE_NO_WARNINGS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <shellapi.h>
#endif
#include <cstdlib>

#include "ConEntry.h"

using namespace chira;

[[maybe_unused]]
static ConCommand open_config_dir{"open_config_dir", "Opens the config directory in the OS's graphical file browser.", [] { // NOLINT(cert-err58-cpp)
    std::string dir = Config::getConfigDirectory().data();
#if defined(CHIRA_PLATFORM_WINDOWS)
    ShellExecute(nullptr, "open", dir.c_str(), nullptr, nullptr, 1);
#elif defined(CHIRA_PLATFORM_APPLE)
    dir = "open " + dir;
    system(dir.c_str());
#elif defined(CHIRA_PLATFORM_LINUX)
    dir = "xdg-open " + dir;
    system(dir.c_str());
#else
    #error "fixme: need code for specific platform!"
#endif
}};

std::string_view Config::getConfigDirectory() {
    // e.g. "C:\Users\<User>\AppData\Local\ChiraEngine\"
    // this will not change while the program is running (or ever for that matter)
    static std::string pathString;
    if (!pathString.empty())
        return pathString;

#ifndef CHIRA_PLATFORM_WINDOWS
    const char* homePath = getenv("HOME");
    if (!homePath) {
        if (passwd* pwd = getpwuid(getuid()))
            homePath = pwd->pw_dir;
    }
    pathString.append(homePath);
#if defined(CHIRA_PLATFORM_APPLE)
    pathString.append("/Library/Application Support/ChiraEngine/");
#elif defined(CHIRA_PLATFORM_LINUX)
    pathString.append("/.local/ChiraEngine/");
#else
    #error "fixme: need code for specific platform!"
#endif
#else // We are on Windows
    // Get the environment variable for the user on Windows
    pathString = getenv("USERPROFILE");
    pathString.append(R"(\AppData\Local\ChiraEngine\)");
#endif
    return pathString;
}
