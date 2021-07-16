#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/debugMeshLoader.h"
#include "../src/render/freecam.h"

int main() {
    engine engine;

    engine.getSettingsLoader()->setValue("engine", "title", std::string("Demo Window"), true, false);

    engine.addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, [](class engine* e) {
        e->stop();
    }));
    engine.addKeybind(keybind(GLFW_KEY_1, GLFW_PRESS, [](class engine* e) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }));
    engine.addKeybind(keybind(GLFW_KEY_2, GLFW_PRESS, [](class engine* e) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }));
    engine.addKeybind(keybind(GLFW_KEY_GRAVE_ACCENT, GLFW_PRESS, [](class engine* e) {
        e->showConsole(!e->getConsole()->getEnabled());
    }));

    shader triangleShader(engine.getResourcesDirectory() + "shaders/unlit.vsh",
                          engine.getResourcesDirectory() + "shaders/unlit.fsh");
    engine.addShader("triangle", &triangleShader);

    texture2d triangleTex("resources/demo/textures/crate.jpg", GL_RGB);
    engine.addTexture("triangle", &triangleTex);

    debugMeshLoader meshLoader;
    mesh triangleMesh(&meshLoader, "cube");
    engine.addMesh("triangle", &triangleMesh);

    freecam player{&engine, glm::vec3(0.0f, 0.0f, 0.0f)};
    engine.setCamera(&player);

    engine.addInitFunction([](class engine* e) {
        e->getShader("triangle")->use();
        e->getShader("triangle")->setUniform("ourTexture", 0);
#if DEBUG
        glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
#endif
    });

    engine.addRenderFunction([](class engine* e) {
        e->getTexture("triangle")->use();
        *e->getMesh("triangle")->getModel() = glm::rotate(
                *e->getMesh("triangle")->getModel(),
                (float)(0.5f * e->getDeltaTime()), glm::vec3(1.0f, 0.0f, 0.0f));
        e->getMesh("triangle")->render(e->getShader("triangle"));
    });

    engine.init();
    engine.run();
}
