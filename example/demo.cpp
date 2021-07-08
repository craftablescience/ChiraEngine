#include "../src/core/engine.h"

static engine obj;

static void stopEngine() {
    obj.stop();
}

int main() {
    keybind esc(GLFW_KEY_ESCAPE, GLFW_PRESS, stopEngine);
    engine::inputManager.addKeybind(esc);
    obj.start();
}
