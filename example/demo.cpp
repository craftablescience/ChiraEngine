#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/debugMeshLoader.h"

// temp
static glm::mat4 mat(1.0);

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
#if DEBUG
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
#endif
}

static void render(engine* engine) {
    engine->getTexture("triangle")->use();
    engine->getShader("triangle")->setUniform("p", &mat);
    engine->getShader("triangle")->setUniform("v", &mat);
    engine->getMesh("triangle")->render(engine->getShader("triangle"));
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

    shader triangleShader(engine.getResourcesDirectory() + "shaders/unlit.vsh",
                          engine.getResourcesDirectory() + "shaders/unlit.fsh");
    engine.addShader("triangle", &triangleShader);

    texture2d triangleTex("resources/demo/textures/crate.jpg", GL_RGB);
    engine.addTexture("triangle", &triangleTex);

    debugMeshLoader meshLoader;
    mesh triangleMesh(&meshLoader, "cube");
    engine.addMesh("triangle", &triangleMesh);

    engine.addInitFunction(init);
    engine.addRenderFunction(render);
    //engine.addStopFunction(stop);

    engine.init();
    engine.run();
}
