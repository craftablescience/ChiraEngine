#include "Config.h"

#if defined(CHIRA_PLATFORM_APPLE) || defined(CHIRA_PLATFORM_LINUX)
    // Using std::ignore to absorb system()'s return value
    #include <tuple>
#endif

#include <core/Platform.h>
#ifdef CHIRA_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <shellapi.h>
#else // Should work on any other platform besides Windows? macOS and Linux work at least
    #include <unistd.h>
    #include <pwd.h>
#endif
#include <cstdlib>

#include "ConEntry.h"

using namespace chira;

[[maybe_unused]]
ConCommand open_config_dir{"open_config_dir", "Opens the config directory in the OS's graphical file browser.", [] {
    std::string dir = Config::getConfigDirectory().data();
#if defined(CHIRA_PLATFORM_WINDOWS)
    ShellExecute(nullptr, "open", dir.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(CHIRA_PLATFORM_APPLE)
    dir = "open " + dir;
    std::ignore = system(dir.c_str());
#elif defined(CHIRA_PLATFORM_LINUX)
    dir = "xdg-open " + dir;
    std::ignore = system(dir.c_str());
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
        if (passwd* pwd = getpwuid(getuid())) {
            homePath = pwd->pw_dir;
        }
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

std::string Config::getConfigFile(std::string_view file) {
    return std::string{Config::getConfigDirectory()} + file.data();
}
