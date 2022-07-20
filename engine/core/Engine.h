#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <string_view>

#include <loader/settings/JSONSettingsLoader.h>
#include <entity/root/Window.h>
#include <math/Color.h>

namespace chira {

const std::string ENGINE_FILESYSTEM_PATH = "engine"; // NOLINT(cert-err58-cpp)

class Engine {
public:
    /// Ran at the very start of your program. Readies the engine for you to add features before init().
    static void preInit(std::string_view configPath = "config.json");
    static void init(bool windowStartsVisible = true);
    static void run();
    [[nodiscard]] static JSONSettingsLoader* getSettings();
    /// Returns a pointer to the main window of the application.
    [[nodiscard]] static Window* getWindow();
    [[nodiscard]] static bool isStarted();
    /// Only guaranteed to work after run() in a render method
    [[nodiscard]] static double getDeltaTime();
private:
    static inline std::unique_ptr<JSONSettingsLoader> settingsLoader;
    static inline std::unique_ptr<Window> window;
    static inline bool started = false;
    static inline double lastTime = 0.0, currentTime = 0.0;
    static void setSettingsDefaults();
};

} // namespace chira
