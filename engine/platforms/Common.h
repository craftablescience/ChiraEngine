#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <core/math/Color.h>
#include <render/backend/RenderDevice.h>

namespace chira {

class Engine {
public:
    Engine() = delete;

    /// Ran at the very start of your program. Initializes modules and readies the engine main window.
    static void init(int argc, const char* argv[], bool onlyInitEssentials = false);
    static void run();

    /// Returns a pointer to the main window of the application.
    [[nodiscard]] static Device::WindowHandle* getMainWindow() { return Engine::mainWindow; }
    [[nodiscard]] static bool isStarted();
    /// Only guaranteed to work after run() in a render method
    [[nodiscard]] static uint64_t getDeltaTicks();
private:
    static inline Device::WindowHandle* mainWindow = nullptr;
    static inline bool started = false;
    static inline uint64_t lastTime = 0, currentTime = 0;
};

} // namespace chira
