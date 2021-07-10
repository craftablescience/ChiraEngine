#ifndef BASICGAMEENGINE_ENGINE_H
#define BASICGAMEENGINE_ENGINE_H


#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include "../input/input.h"
#include "../utility/image.h"
#include "../shader/shader.h"

class engine {
public:
    void init();
    void start(const std::string& iconPath = "");
    void render();
    void stop() const;
    void addShader(shader* s);
    // NOTE: PNGs must have a bit depth of 8 or less* (less not tested)
    void setIcon(const std::string& iconPath);
    void setInputManager(input* newInput);
    input* getInputManager();
private:
    GLFWwindow* window = nullptr;
    input inputManager;
    std::list<shader*> shaders;
    bool started = false;
    void processInput(GLFWwindow* inputWindow);
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};


#endif //BASICGAMEENGINE_ENGINE_H
