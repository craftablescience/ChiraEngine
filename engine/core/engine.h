#pragma once

#include <functional>
#include <vector>
#include <memory>

#include <loader/settings/abstractSettingsLoader.h>
#ifdef CHIRA_BUILD_WITH_ANGELSCRIPT
#include <script/angelscriptProvider.h>
#endif
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
#ifdef CHIRA_BUILD_WITH_ANGELSCRIPT
        [[nodiscard]] static AngelscriptProvider* getAngelscriptProvider();
#endif
        [[nodiscard]] static AbstractSoundManager* getSoundManager();
        static void setSoundManager(AbstractSoundManager* newSoundManager);
        [[nodiscard]] static AbstractSettingsLoader* getSettingsLoader();
        static void setSettingsLoader(AbstractSettingsLoader* newSettingsLoader);
        /// Returns a pointer to the main window of the application.
        [[nodiscard]] static Window* getWindow();
        [[nodiscard]] static bool isStarted();
        /// Note: only guaranteed to work after run() in a render method
        [[nodiscard]] static double getDeltaTime();
    private:
#ifdef CHIRA_BUILD_WITH_ANGELSCRIPT
        static inline std::unique_ptr<AngelscriptProvider> angelscript;
#endif
        static inline std::unique_ptr<AbstractSoundManager> soundManager;
        static inline std::unique_ptr<AbstractSettingsLoader> settingsLoader;
        static inline std::unique_ptr<Window> window;
        static inline bool started = false;
        static inline double lastTime = 0.0, currentTime = 0.0;
        static void setSettingsLoaderDefaults();
    };
}
