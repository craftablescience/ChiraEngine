#include "../src/core/engine.h"

static engine obj;

static void stopEngine() {
    obj.stop();
}

int main() {
    keybind esc(GLFW_KEY_ESCAPE, GLFW_PRESS, stopEngine);
    obj.getInputManager()->addKeybind(esc);
    obj.start("resources/images/icon.png");
}
