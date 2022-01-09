#pragma once

#include <glad/gl.h>
#include <map>
#include <functional>
#include <vector>
#include <memory>
#include <loader/settings/abstractSettingsLoader.h>
#include <script/abstractScriptProvider.h>
#include <script/angelscriptProvider.h>
#include <sound/abstractSoundManager.h>
#include <physics/abstractPhysicsProvider.h>
#include <entity/3d/root.h>
#include <utility/math/color.h>

class GLFWwindow;

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
        static void render();
        static void stop();

        static void addInitFunction(const std::function<void()>& init);
        static void addRenderFunction(const std::function<void()>& render);
        static void addStopFunction(const std::function<void()>& stop);

        static void setBackgroundColor(ColorRGB color);
        static glm::vec2 getWindowSize();
        static int getWindowWidth();
        static int getWindowHeight();
        static void setWindowSize(int width, int height);
        static void shouldStopAfterThisFrame(bool yes);

        static AngelscriptProvider* getAngelscriptProvider();
        static void setSoundManager(AbstractSoundManager* newSoundManager);
        static AbstractSoundManager* getSoundManager();

        static AbstractSettingsLoader* getSettingsLoader();
        static void setSettingsLoader(AbstractSettingsLoader* newSettingsLoader);
        static AbstractPhysicsProvider* getPhysicsProvider();
        static void setPhysicsProvider(AbstractPhysicsProvider* newPhysicsProvider);

        [[nodiscard]] static Root* getRoot();

        static void callRegisteredFunctions(const std::vector<std::function<void()>>* list);

        [[nodiscard]] static GLFWwindow* getWindow();
        [[nodiscard]] static bool isStarted();
        /// Note: only guaranteed to work after run() in a render method
        [[nodiscard]] static double getDeltaTime();
        static void captureMouse(bool capture);
        [[nodiscard]] static bool isMouseCaptured();
        static Console* getConsole();
        static Profiler* getProfiler();
        [[nodiscard]] static bool isIconified();
    private:
        static GLFWwindow* window;
        static std::vector<std::function<void()>> initFunctions;
        static std::vector<std::function<void()>> renderFunctions;
        static std::vector<std::function<void()>> stopFunctions;
        static std::unique_ptr<AngelscriptProvider> angelscript;
        static std::unique_ptr<AbstractSoundManager> soundManager;
        static std::unique_ptr<AbstractSettingsLoader> settingsLoader;
        static std::unique_ptr<AbstractPhysicsProvider> physicsProvider;
        static Root* root;
        static Console* console;
#ifdef DEBUG
        static Profiler* profiler;
#endif
        static bool mouseCaptured;
        static bool started;
        static bool iconified;
        static double lastTime, currentTime, lastMouseX, lastMouseY;

        static void setSettingsLoaderDefaults();
        /// Note: Images must have a bit depth of 8, in RGB format
        static void setIcon(const std::string& iconPath);
        static void displaySplashScreen();
        static void keyboardCallback(GLFWwindow* w, int key, int scancode, int action, int mods);
        static void keyboardRepeatingCallback();
        static void mouseButtonCallback(GLFWwindow* w, int key, int action, int mods);
        static void mouseButtonRepeatingCallback();
        static void mouseMovementCallback(GLFWwindow* w, double xPos, double yPos);
        static void mouseScrollCallback(GLFWwindow* w, double x, double y);
    };
}
