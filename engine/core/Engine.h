#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <loader/settings/JSONSettingsLoader.h>
#include <math/Color.h>
#include <render/backend/RenderDevice.h>

namespace chira {

const std::string ENGINE_FILESYSTEM_PATH = "engine";

class Engine {
public:
    Engine() = delete;

    /// Ran at the very start of your program. Readies the engine for you to add features before init().
    static void preInit(int argc, const char* const argv[]);
    static void init(bool visibleSplashScreen = true);
    static void run();

    /// Returns a pointer to the main window of the application.
    [[nodiscard]] static Device::WindowHandle* getDevice() { return Engine::mainWindow; }
    [[nodiscard]] static Frame* getRoot() { return Device::getWindowFrame(Engine::mainWindow); }
    [[nodiscard]] static bool isStarted();
    /// Only guaranteed to work after run() in a render method
    [[nodiscard]] static uint64_t getDeltaTicks();
private:
    static inline Device::WindowHandle* mainWindow = nullptr;
    static inline bool started = false;
    static inline uint64_t lastTime = 0, currentTime = 0;
};

} // namespace chira
