#pragma once

#include <functional>
#include <vector>
#include <memory>

//todo(viewport): move to Window
#define IMGUI_USER_CONFIG <config/imguiConfig.h>
#include <imgui.h>

#include <loader/settings/abstractSettingsLoader.h>
#include <script/angelscriptProvider.h>
#include <sound/abstractSoundManager.h>
#include <physics/abstractPhysicsProvider.h>
#include <entity/root/window.h>
#include <utility/math/color.h>

namespace chira {
    class Console;
    class Profiler;

    const std::string ENGINE_FILESYSTEM_PATH = "engine"; // NOLINT(cert-err58-cpp)

    class Engine {
    public:
        /// Ran at the very start of your program. Readies the engine for you to add features before init().
        static void preInit(const std::string& configPath = "settings.json");
        static void init();
        static void run();
        /// Should not be called manually! Use <code>Engine::shouldStopAfterThisFrame(true)</code> instead!
        static void stop();

        static void addInitFunction(const std::function<void()>& init);
        static void addRenderFunction(const std::function<void()>& render);
        static void addStopFunction(const std::function<void()>& stop);

        [[nodiscard]] static AngelscriptProvider* getAngelscriptProvider();
        [[nodiscard]] static AbstractSoundManager* getSoundManager();
        static void setSoundManager(AbstractSoundManager* newSoundManager);
        [[nodiscard]] static AbstractSettingsLoader* getSettingsLoader();
        static void setSettingsLoader(AbstractSettingsLoader* newSettingsLoader);

        [[nodiscard]] static Window* getWindow();
        [[nodiscard]] static Console* getConsole();
        [[nodiscard]] static Profiler* getProfiler();

        [[nodiscard]] static bool isStarted();
        /// Note: only guaranteed to work after run() in a render method
        [[nodiscard]] static double getDeltaTime();
    private:
        static inline std::vector<std::function<void()>> initFunctions, renderFunctions, stopFunctions;
        static inline std::unique_ptr<AngelscriptProvider> angelscript;
        static inline std::unique_ptr<AbstractSoundManager> soundManager;
        static inline std::unique_ptr<AbstractSettingsLoader> settingsLoader;
        static inline Window* root;
        static inline Console* console;
#ifdef DEBUG
        static inline Profiler* profiler;
#endif
        static inline bool started = false;
        static inline double lastTime = 0.0, currentTime = 0.0;

        static void setSettingsLoaderDefaults();
        static void callRegisteredFunctions(const std::vector<std::function<void()>>& list);
    };
}
