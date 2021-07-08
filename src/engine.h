#ifndef BASICGAMEENGINE_ENGINE_H
#define BASICGAMEENGINE_ENGINE_H


#include <GLFW/glfw3.h>

class engine {
public:
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void start();
    void render();
    void stop() const;
private:
    GLFWwindow* window = nullptr;
};


#endif //BASICGAMEENGINE_ENGINE_H
