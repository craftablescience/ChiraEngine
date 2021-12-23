#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <map>
#include <functional>
#include <vector>
#include <memory>
#include <input/keybind.h>
#include <input/mousebind.h>
#include <loader/settings/abstractSettingsLoader.h>
#include <script/abstractScriptProvider.h>
#include <script/angelscriptProvider.h>
#include <sound/abstractSoundManager.h>
#include <physics/abstractPhysicsProvider.h>
#include <entity/3d/root.h>

namespace chira {
    class console;
    class profiler;

    const std::string ENGINE_FILESYSTEM_PATH = "engine"; // NOLINT(cert-err58-cpp)

    class engine {
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

        static void setBackgroundColor(float r, float g, float b, float a);
        static glm::vec2 getWindowSize();
        static int getWindowWidth();
        static int getWindowHeight();
        static void setWindowSize(int width, int height);

        static void addKeybind(const keybind& keybind);
        static std::vector<keybind>* getKeybinds();
        static void addMousebind(const mousebind& mousebind);
        static std::vector<mousebind>* getMousebinds();

        static angelscriptProvider* getAngelscriptProvider();
        static void setSoundManager(abstractSoundManager* newSoundManager);
        static abstractSoundManager* getSoundManager();

        static abstractSettingsLoader* getSettingsLoader();
        static void setSettingsLoader(abstractSettingsLoader* newSettingsLoader);
        static abstractPhysicsProvider* getPhysicsProvider();
        static void setPhysicsProvider(abstractPhysicsProvider* newPhysicsProvider);

        [[nodiscard]] static root* getRoot();

        static void callRegisteredFunctions(const std::vector<std::function<void()>>* list);

        [[nodiscard]] static GLFWwindow* getWindow();
        [[nodiscard]] static bool isStarted();
        /// Note: only guaranteed to work after run() in a render method
        [[nodiscard]] static double getDeltaTime();
        static void captureMouse(bool capture);
        [[nodiscard]] static bool isMouseCaptured();
        static console* getConsole();
        static profiler* getProfiler();
        [[nodiscard]] static bool isIconified();
    private:
        static GLFWwindow* window;
        static std::vector<std::function<void()>> initFunctions;
        static std::vector<std::function<void()>> renderFunctions;
        static std::vector<std::function<void()>> stopFunctions;
        static std::unique_ptr<angelscriptProvider> angelscript;
        static std::unique_ptr<abstractSoundManager> soundManager;
        static std::vector<keybind> keybinds;
        static std::vector<mousebind> mousebinds;
        static std::unique_ptr<abstractSettingsLoader> settingsLoader;
        static std::unique_ptr<abstractPhysicsProvider> physicsProvider;
        static root* treeRoot;
        static console* consoleUI;
#ifdef DEBUG
        static profiler* profilerUI;
#endif
        static bool mouseCaptured;
        static bool started;
        static bool iconified;
        static double lastTime, currentTime, lastMouseX, lastMouseY;

        static void setSettingsLoaderDefaults();
        /// Note: PNGs must have a bit depth of 8 or less* (less not tested)
        static void setIcon(const std::string& iconPath);
        static void displaySplashScreen();
        static void errorCallback(int error, const char* description);
        static void framebufferSizeCallback(GLFWwindow* w, int width, int height);
        static void keyboardCallback(GLFWwindow* w, int key, int scancode, int action, int mods);
        static void keyboardRepeatingCallback();
        static void mouseButtonCallback(GLFWwindow* w, int button, int action, int mods);
        static void mouseButtonRepeatingCallback();
        static void mouseMovementCallback(GLFWwindow* w, double xPos, double yPos);
        static void mouseScrollCallback(GLFWwindow* w, double xPos, double yPos);
        static void windowIconifyCallback(GLFWwindow* w, int isIconified);
    };
}
