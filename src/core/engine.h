#ifndef BASICGAMEENGINE_ENGINE_H
#define BASICGAMEENGINE_ENGINE_H


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
#include "../ui/console.h"
#include "../utility/logger.h"

class keybind;
class mousebind;
class angelscriptProvider;

class engine {
public:
    engine();
    virtual ~engine();
    void init();
    void run();
    void render();
    void stop();
    static void setBackgroundColor(float r, float g, float b, float a);
    void addKeybind(const keybind& keybind);
    std::vector<keybind>* getKeybinds();
    void addMousebind(const mousebind& mousebind);
    std::vector<mousebind>* getMousebinds();
    void addShader(const std::string& name, shader* s);
    shader* getShader(const std::string& name);
    void addTexture(const std::string& name, texture* t);
    texture* getTexture(const std::string& name);
    void addMesh(const std::string& name, mesh* t);
    mesh* getMesh(const std::string& name);
    void addScriptProvider(const std::string& name, abstractScriptProvider* scriptProvider);
    abstractScriptProvider* getScriptProvider(const std::string& name);
    void addInitFunction(const std::function<void(engine*)>& init);
    void addRenderFunction(const std::function<void(engine*)>& render);
    void addStopFunction(const std::function<void(engine*)>& stop);
    abstractCamera* getCamera() const;
    void setCamera(abstractCamera* newCamera);
    abstractSettingsLoader* getSettingsLoader();
    void setSettingsLoader(abstractSettingsLoader* settingsLoader);
    void callRegisteredFunctions(const std::vector<std::function<void(engine*)>>* list);
    [[nodiscard]] bool isStarted() const;
    // NOTE: only guaranteed to work after run() in a render method
    double getDeltaTime() const;
    std::string getResourcesDirectory() const;
    void setResourcesDirectory(const std::string& resourcesDirectory);
    void addLogHook(const std::function<void(engine*,const loggerType,const std::string&,const std::string&)>& function);
    void captureMouse(bool capture);
    bool isMouseCaptured() const;
    void showConsole(bool shouldShow);
    console* getConsole();
    void logInfo(const std::string& source, const std::string& message);
    void logInfoImportant(const std::string& source, const std::string& message);
    void logOutput(const std::string& source, const std::string& message);
    void logWarning(const std::string& source, const std::string& message);
    void logError(const std::string& source, const std::string& message);
private:
    GLFWwindow* window = nullptr;
    std::vector<std::function<void(engine*)>> initFunctions;
    std::vector<std::function<void(engine*)>> renderFunctions;
    std::vector<std::function<void(engine*)>> stopFunctions;
    std::vector<std::function<void(engine*,const loggerType,const std::string&,const std::string&)>> loggerFunctions;
    std::map<std::string, std::unique_ptr<abstractScriptProvider>> scriptProviders;
    std::vector<keybind> keybinds;
    std::vector<mousebind> mousebinds;
    std::map<std::string, std::unique_ptr<compilable>> compilableObjects;
    abstractCamera* camera = nullptr;
    bool mouseCaptured = false;
    abstractSettingsLoader* settingsLoader = nullptr;
    logger logger;
    console consoleUI;
    std::string resourcesDirectoryPath;
    bool started = false;
    double lastTime, currentTime, lastMouseX, lastMouseY;
    void setSettingsLoaderDefaults();
    // NOTE: PNGs must have a bit depth of 8 or less* (less not tested)
    void setIcon(const std::string& iconPath);
    void runLogHooks(const loggerType type, const std::string& source, const std::string& message);
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void keyboardRepeatingCallback();
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseButtonRepeatingCallback();
    static void mouseMovementCallback(GLFWwindow* window, double xPos, double yPos);
    static void mouseScrollCallback(GLFWwindow* window, double xPos, double yPos);
};


#endif //BASICGAMEENGINE_ENGINE_H
