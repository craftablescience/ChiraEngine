#pragma once

#include <map>
#include <functional>
#include <vector>
#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <loader/settings/abstractSettingsLoader.h>
#include <script/abstractScriptProvider.h>
#include <script/angelscriptProvider.h>
#include <sound/abstractSoundManager.h>
#include <physics/abstractPhysicsProvider.h>
#include <entity/root.h>
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
        static void stop();

        static void addInitFunction(const std::function<void()>& init);
        static void addRenderFunction(const std::function<void()>& render);
        static void addStopFunction(const std::function<void()>& stop);

        static void setBackgroundColor(ColorRGB color);
        [[nodiscard]] static glm::vec2 getWindowSize();
        [[nodiscard]] static int getWindowWidth();
        [[nodiscard]] static int getWindowHeight();
        static void setWindowSize(int width, int height);
        static void shouldStopAfterThisFrame(bool yes);

        [[nodiscard]] static AngelscriptProvider* getAngelscriptProvider();
        [[nodiscard]] static AbstractSoundManager* getSoundManager();
        static void setSoundManager(AbstractSoundManager* newSoundManager);
        [[nodiscard]] static AbstractSettingsLoader* getSettingsLoader();
        static void setSettingsLoader(AbstractSettingsLoader* newSettingsLoader);

        [[nodiscard]] static Root* getRoot();
        [[nodiscard]] static Console* getConsole();
        [[nodiscard]] static Profiler* getProfiler();

        [[nodiscard]] static bool isStarted();
        /// Note: only guaranteed to work after run() in a render method
        [[nodiscard]] static double getDeltaTime();
        static void captureMouse(bool capture);
        [[nodiscard]] static bool isMouseCaptured();
        [[nodiscard]] static glm::vec2 getMousePosition();
        [[nodiscard]] static bool isIconified();
        /// Note: Images must have a bit depth of 8, in RGB format
        static void setIcon(const std::string& iconPath);
    private:
        static inline GLFWwindow* window;
        static inline std::vector<std::function<void()>> initFunctions, renderFunctions, stopFunctions;
        static inline std::unique_ptr<AngelscriptProvider>     angelscript;
        static inline std::unique_ptr<AbstractSoundManager>    soundManager;
        static inline std::unique_ptr<AbstractSettingsLoader>  settingsLoader;
        static inline Root* root;
        static inline Console* console;
#ifdef DEBUG
        static inline Profiler* profiler;
#endif
        static inline bool mouseCaptured, started, iconified; // false
        static inline double lastTime, currentTime, lastMouseX, lastMouseY; // 0.0

        static void displaySplashScreen();
        static void setSettingsLoaderDefaults();
        static void callRegisteredFunctions(const std::vector<std::function<void()>>& list);
    };
}
