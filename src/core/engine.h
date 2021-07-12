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
#include "../render/abstractCamera.h"

class keybind;

class engine {
public:
    engine();
    void init(const std::string& iconPath = "");
    void run();
    void render();
    void stop();
    void addKeybind(const keybind& keybind);
    void addShader(const std::string& name, shader* s);
    shader* getShader(const std::string& name);
    void addTexture(const std::string& name, texture* t);
    texture* getTexture(const std::string& name);
    void addMesh(const std::string& name, mesh* t);
    mesh* getMesh(const std::string& name);
    void addInitFunction(const std::function<void(engine*)>& init);
    void addRenderFunction(const std::function<void(engine*)>& render);
    void addStopFunction(const std::function<void(engine*)>& stop);
    abstractCamera* getCamera() const;
    void setCamera(abstractCamera* newCamera);
    void callRegisteredFunctions(const std::vector<std::function<void(engine*)>>* list);
    [[nodiscard]] bool isStarted() const;
    // NOTE: only guaranteed to work after run() in a render method
    double getDeltaTime() const;
    std::string getResourcesDirectory() const;
    void setResourcesDirectory(const std::string& resourcesDirectory);
    void captureMouse() const;
    void freeMouse() const;
private:
    GLFWwindow* window = nullptr;
    std::vector<std::function<void(engine*)>> initFunctions;
    std::vector<std::function<void(engine*)>> renderFunctions;
    std::vector<std::function<void(engine*)>> stopFunctions;
    std::vector<keybind> keybinds;
    std::map<std::string, std::unique_ptr<glCompilable>> glObjects;
    abstractCamera* camera;
    std::string resourcesDirectoryPath;
    bool started = false;
    double lastTime, currentTime;
    void processKeybinds();
    // NOTE: PNGs must have a bit depth of 8 or less* (less not tested)
    void setIcon(const std::string& iconPath);
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};


#endif //BASICGAMEENGINE_ENGINE_H
