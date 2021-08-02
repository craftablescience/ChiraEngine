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
#include "world.h"
#include "../render/abstractMaterial.h"

class keybind;
class mousebind;

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
    static void addShader(const std::string& name, shader* s);
    static shader* getShader(const std::string& name);
    static void addTexture(const std::string& name, texture* t);
    static texture* getTexture(const std::string& name);
    static void addMesh(const std::string& name, mesh* t);
    static mesh* getMesh(const std::string& name);
    static void addMaterial(const std::string& name, abstractMaterial* t);
    static abstractMaterial* getMaterial(const std::string& name);
    void addScriptProvider(const std::string& name, abstractScriptProvider* scriptProvider);
    abstractScriptProvider* getScriptProvider(const std::string& name);
    void setSoundManager(abstractSoundManager* newSoundManager);
    abstractSoundManager* getSoundManager();
    void addInitFunction(const std::function<void(engine*)>& init);
    void addRenderFunction(const std::function<void(engine*)>& render);
    void addStopFunction(const std::function<void(engine*)>& stop);
    abstractSettingsLoader* getSettingsLoader();
    void setSettingsLoader(abstractSettingsLoader* settingsLoader);
    class world* getWorld();
    void setWorld(world* newWorld);
    void callRegisteredFunctions(const std::vector<std::function<void(engine*)>>* list);
    [[nodiscard]] bool isStarted() const;
    // NOTE: only guaranteed to work after run() in a render method
    [[nodiscard]] double getDeltaTime() const;
    static void addLogHook(const std::function<void(const loggerType,const std::string&,const std::string&)>& function);
    void captureMouse(bool capture);
    [[nodiscard]] bool isMouseCaptured() const;
    void showConsole(bool shouldShow);
    console* getConsole();
    static void logInfo(const std::string& source, const std::string& message);
    static void logInfoImportant(const std::string& source, const std::string& message);
    static void logOutput(const std::string& source, const std::string& message);
    static void logWarning(const std::string& source, const std::string& message);
    static void logError(const std::string& source, const std::string& message);
private:
    GLFWwindow* window = nullptr;
    std::vector<std::function<void(engine*)>> initFunctions{};
    std::vector<std::function<void(engine*)>> renderFunctions{};
    std::vector<std::function<void(engine*)>> stopFunctions{};
    static inline std::vector<std::function<void(const loggerType,const std::string&,const std::string&)>> loggerFunctions{};
    std::map<std::string, std::unique_ptr<abstractScriptProvider>> scriptProviders{};
    std::unique_ptr<abstractSoundManager> soundManager = nullptr;
    std::vector<keybind> keybinds{};
    std::vector<mousebind> mousebinds{};
    static inline std::map<const std::string, std::unique_ptr<shader>> shaders{};
    static inline std::map<const std::string, std::unique_ptr<texture>> textures{};
    static inline std::map<const std::string, std::unique_ptr<mesh>> meshes{};
    static inline std::map<const std::string, std::unique_ptr<abstractMaterial>> materials{};
    bool mouseCaptured = false;
    std::unique_ptr<abstractSettingsLoader> settingsLoader = nullptr;
    std::unique_ptr<world> worldPtr = nullptr;
    static inline logger chiraLogger{};
    console consoleUI{};
    bool started = false;
    double lastTime, currentTime, lastMouseX, lastMouseY;
    void setSettingsLoaderDefaults();
    // NOTE: PNGs must have a bit depth of 8 or less* (less not tested)
    void setIcon(const std::string& iconPath);
    static void runLogHooks(loggerType type, const std::string& source, const std::string& message);
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void keyboardRepeatingCallback();
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseButtonRepeatingCallback();
    static void mouseMovementCallback(GLFWwindow* window, double xPos, double yPos);
    static void mouseScrollCallback(GLFWwindow* window, double xPos, double yPos);
};
