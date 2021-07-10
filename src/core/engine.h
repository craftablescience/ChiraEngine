#ifndef BASICGAMEENGINE_ENGINE_H
#define BASICGAMEENGINE_ENGINE_H


#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <map>
#include <functional>
#include <list>
#include <memory>
#include "../render/shader.h"
#include "../render/texture.h"
#include "../input/keybind.h"

class keybind;

class engine {
public:
    void init();
    void start(const std::string& iconPath = "");
    void render();
    void stop();
    void addKeybind(const keybind& keybind);
    void addShader(const std::string& name, shader* s);
    shader* getShader(const std::string& name);
    void addTexture(const std::string& name, texture* t);
    texture* getTexture(const std::string& name);
    void addInitFunction(const std::function<void(engine*)>& init);
    void addRenderFunction(const std::function<void(engine*)>& render);
    void addStopFunction(const std::function<void(engine*)>& stop);
    // NOTE: PNGs must have a bit depth of 8 or less* (less not tested)
    void setIcon(const std::string& iconPath);
    void callRegisteredFunctions(const std::list<std::function<void(engine*)>>* list);
private:
    GLFWwindow* window = nullptr;
    std::list<std::function<void(engine*)>> initFunctions;
    std::list<std::function<void(engine*)>> renderFunctions;
    std::list<std::function<void(engine*)>> stopFunctions;
    std::list<keybind> keybinds;
    std::map<std::string, std::unique_ptr<glCompilable>> glObjects;
    bool started = false;
    void processInput(GLFWwindow* inputWindow);
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};


#endif //BASICGAMEENGINE_ENGINE_H
