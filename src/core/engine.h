#pragma once

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <map>
#include <functional>
#include <vector>
#include <memory>
#include "../render/shader.h"
#include "../render/texture.h"
#include "../render/mesh.h"
#include "../input/keybind.h"
#include "../input/mousebind.h"
#include "../loader/abstractSettingsLoader.h"
#include "../render/abstractCamera.h"
#include "../script/abstractScriptProvider.h"
#include "../script/angelscriptProvider.h"
#include "../sound/abstractSoundManager.h"
#include "../ui/console.h"
#include "../utility/logger.h"
#include "../render/world.h"
#include "../render/material.h"

class keybind;
class mousebind;
class world;

class engine {
public:
    explicit engine(const std::string& configPath = "settings.json");
    void init();
    void run();
    void render();
    void stop();
    static void setBackgroundColor(float r, float g, float b, float a);
    void addKeybind(const keybind& keybind);
    std::vector<keybind>* getKeybinds();
    void addMousebind(const mousebind& mousebind);
    std::vector<mousebind>* getMousebinds();
    void addScriptProvider(const std::string& name, abstractScriptProvider* scriptProvider);
    abstractScriptProvider* getScriptProvider(const std::string& name);
    void setSoundManager(abstractSoundManager* newSoundManager);
    abstractSoundManager* getSoundManager();
    void addInitFunction(const std::function<void(engine*)>& init);
    void addRenderFunction(const std::function<void(engine*)>& render);
    void addStopFunction(const std::function<void(engine*)>& stop);
    static abstractSettingsLoader* getSettingsLoader();
    static void setSettingsLoader(abstractSettingsLoader* settingsLoader);
    world* getWorld();
    void setWorld(world* newWorld);
    void callRegisteredFunctions(const std::vector<std::function<void(engine*)>>* list);
    [[nodiscard]] const GLFWwindow* getWindow() const;
    [[nodiscard]] bool isStarted() const;
    // NOTE: only guaranteed to work after run() in a render method
    [[nodiscard]] double getDeltaTime() const;
    void captureMouse(bool capture);
    [[nodiscard]] bool isMouseCaptured() const;
    void showConsole(bool shouldShow);
    console* getConsole();
private:
    GLFWwindow* window = nullptr;
    std::vector<std::function<void(engine*)>> initFunctions{};
    std::vector<std::function<void(engine*)>> renderFunctions{};
    std::vector<std::function<void(engine*)>> stopFunctions{};
    std::map<std::string, std::unique_ptr<abstractScriptProvider>> scriptProviders{};
    std::unique_ptr<abstractSoundManager> soundManager = nullptr;
    std::vector<keybind> keybinds{};
    std::vector<mousebind> mousebinds{};
    static inline std::unique_ptr<abstractSettingsLoader> settingsLoader = nullptr;
    bool mouseCaptured = false;
    std::unique_ptr<world> worldPtr = nullptr;
    console consoleUI{};
    bool started = false;
    double lastTime, currentTime, lastMouseX, lastMouseY;
    static void setSettingsLoaderDefaults();
    // NOTE: PNGs must have a bit depth of 8 or less* (less not tested)
    void setIcon(const std::string& iconPath);
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void keyboardRepeatingCallback();
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseButtonRepeatingCallback();
    static void mouseMovementCallback(GLFWwindow* window, double xPos, double yPos);
    static void mouseScrollCallback(GLFWwindow* window, double xPos, double yPos);
};
