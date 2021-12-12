#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <map>
#include <functional>
#include <vector>
#include <memory>
#include <render/shader.h>
#include <render/texture.h>
#include <input/keybind.h>
#include <input/mousebind.h>
#include <loader/abstractSettingsLoader.h>
#include <script/abstractScriptProvider.h>
#include <script/angelscriptProvider.h>
#include <sound/abstractSoundManager.h>
#include <entity/imgui/console/console.h>
#include <utility/logger.h>
#include <render/material.h>
#include <physics/abstractPhysicsProvider.h>
#include <entity/3d/root.h>

namespace chira {
    const std::string ENGINE_FILESYSTEM_PATH = "resources/engine"; // NOLINT(cert-err58-cpp)

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

        [[nodiscard]] static const GLFWwindow* getWindow();
        [[nodiscard]] static bool isStarted();
        /// Note: only guaranteed to work after run() in a render method
        [[nodiscard]] static double getDeltaTime();
        static void captureMouse(bool capture);
        [[nodiscard]] static bool isMouseCaptured();
        static void showConsole(bool shouldShow);
        static console* getConsole();
        [[nodiscard]] static bool isIconified();
    private:
        static inline GLFWwindow* window = nullptr;
        static inline std::vector<std::function<void()>> initFunctions{};
        static inline std::vector<std::function<void()>> renderFunctions{};
        static inline std::vector<std::function<void()>> stopFunctions{};
        static inline std::unique_ptr<angelscriptProvider> angelscript = nullptr;
        static inline std::unique_ptr<abstractSoundManager> soundManager = nullptr;
        static inline std::vector<keybind> keybinds{};
        static inline std::vector<mousebind> mousebinds{};
        static inline std::unique_ptr<abstractSettingsLoader> settingsLoader = nullptr;
        static inline std::unique_ptr<abstractPhysicsProvider> physicsProvider = nullptr;
        static inline std::unique_ptr<root> treeRoot = nullptr;
        static inline console* consoleUI = nullptr;
        static inline bool mouseCaptured = false;
        static inline bool started = false;
        static inline bool iconified = false;
        static inline double lastTime, currentTime, lastMouseX, lastMouseY;
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
