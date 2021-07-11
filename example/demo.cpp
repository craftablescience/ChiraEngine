#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/debugMeshLoader.h"

static void stopEngine(engine* engine) {
    engine->stop();
}

static void enableWireframe(engine* engine) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static void disableWireframe(engine* engine) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static void init(engine* engine) {
    engine->getShader("triangle")->use();
    engine->getShader("triangle")->setUniform("ourTexture", 0);
}

static void render(engine* engine) {
    engine->getTexture("triangle")->use();
    engine->getShader("triangle")->use();
    engine->getMesh("triangle")->render();
}

/*
static void stop(engine* engine) {
    // mostly managed by the engine now
}
*/

int main() {
    engine engine;

    keybind esc(GLFW_KEY_ESCAPE, GLFW_PRESS, stopEngine);
    keybind noWire(GLFW_KEY_1, GLFW_PRESS, disableWireframe);
    keybind wire(GLFW_KEY_2, GLFW_PRESS, enableWireframe);
    engine.addKeybind(esc);
    engine.addKeybind(noWire);
    engine.addKeybind(wire);

    shader triangleShader("resources/shaders/triangle.vsh", "resources/shaders/triangle.fsh");
    engine.addShader("triangle", &triangleShader);

    texture2d triangleTex("resources/images/crate.jpg", GL_RGB);
    engine.addTexture("triangle", &triangleTex);

    debugMeshLoader meshLoader;
    mesh triangleMesh(&meshLoader, "debug, this parameter is unused");
    engine.addMesh("triangle", &triangleMesh);

    engine.addInitFunction(init);
    engine.addRenderFunction(render);
    //engine.addStopFunction(stop);

    engine.init("resources/images/icon.png");
    engine.run();
}
