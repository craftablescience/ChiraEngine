#include "../src/core/engine.h"

static engine obj;

static void stopEngine() {
    obj.stop();
}

int main() {
    keybind esc(GLFW_KEY_ESCAPE, GLFW_PRESS, stopEngine);
    obj.getInputManager()->addKeybind(esc);

    shader triangle("resources/shaders/triangle.vsh", "resources/shaders/triangle.fsh");
    obj.addShader(&triangle);

    obj.start("resources/images/icon.png");
}
