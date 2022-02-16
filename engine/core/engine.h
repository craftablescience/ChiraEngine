#pragma once

#include <functional>
#include <vector>
#include <memory>

#include <loader/settings/abstractSettingsLoader.h>
#include <script/angelscriptProvider.h>
#include <sound/abstractSoundManager.h>
#include <physics/abstractPhysicsProvider.h>
#include <entity/root/window.h>
#include <utility/math/color.h>

namespace chira {
    const std::string ENGINE_FILESYSTEM_PATH = "engine"; // NOLINT(cert-err58-cpp)

    class Engine {
    public:
        /// Ran at the very start of your program. Readies the engine for you to add features before init().
        static void preInit(const std::string& configPath = "settings.json");
        static void init(const std::function<void()>& callbackOnInit = []{}, bool windowStartsVisible = true);
        static void run(const std::function<void()>& callbackOnStop = []{});
        [[nodiscard]] static AngelscriptProvider* getAngelscriptProvider();
        [[nodiscard]] static AbstractSoundManager* getSoundManager();
        static void setSoundManager(AbstractSoundManager* newSoundManager);
        [[nodiscard]] static AbstractSettingsLoader* getSettingsLoader();
        static void setSettingsLoader(AbstractSettingsLoader* newSettingsLoader);
        /// Returns a pointer to the main window of the application.
        [[nodiscard]] static Window* getWindow();
#ifdef CHIRA_BUILD_WITH_MULTIWINDOW
        [[nodiscard]] static Window* getWindow(const std::string& name);
        static std::string addWindow(const std::string& title, int width, int height, bool fullscreen = false, ColorRGB backgroundColor_ = {}, bool smoothResize = true, bool startVisible = true);
        static void removeWindow(const std::string& name);
#endif
        [[nodiscard]] static bool isStarted();
        /// Note: only guaranteed to work after run() in a render method
        [[nodiscard]] static double getDeltaTime();
    private:
        static inline std::unique_ptr<AngelscriptProvider> angelscript;
        static inline std::unique_ptr<AbstractSoundManager> soundManager;
        static inline std::unique_ptr<AbstractSettingsLoader> settingsLoader;
        static inline std::vector<std::unique_ptr<Window>> windows;
        static inline bool started = false;
        static inline double lastTime = 0.0, currentTime = 0.0;
        static void setSettingsLoaderDefaults();
    };
}
