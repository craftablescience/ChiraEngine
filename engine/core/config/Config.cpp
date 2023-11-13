#include "Config.h"

#include "../Platform.h"
#ifdef CHIRA_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <shellapi.h>
#else
	#include <cstdlib>
#endif

#include "ConEntry.h"

using namespace chira;

[[maybe_unused]]
ConCommand open_config_dir{"open_config_dir", "Opens the config directory in the OS's graphical file browser.", [] {
	// todo(fs)
    std::string dir{"C:\\"};
#if defined(CHIRA_PLATFORM_WINDOWS)
    ShellExecute(nullptr, "open", dir.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(CHIRA_PLATFORM_APPLE)
    dir = "open " + dir;
    (void) system(dir.c_str());
#elif defined(CHIRA_PLATFORM_LINUX)
    dir = "xdg-open " + dir;
    (void) system(dir.c_str());
#else
    #error "fixme: need code for specific platform!"
#endif
}};
