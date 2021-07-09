#ifndef BASICGAMEENGINE_ENGINE_H
#define BASICGAMEENGINE_ENGINE_H


#include <GLFW/glfw3.h>
#include <iostream>
#include "../input/input.h"

class engine {
public:
    static input inputManager;
    void start();
    void render();
    void stop() const;
private:
    GLFWwindow* window = nullptr;
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
};


#endif //BASICGAMEENGINE_ENGINE_H
