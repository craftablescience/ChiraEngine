#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/objMeshLoader.h"
#include "../src/render/freecam.h"

int main() {
    engine engine;
    objMeshLoader objMeshLoader;
    engine.getSettingsLoader()->setValue("engine", "title", std::string("Demo Window"), true, true);

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

    shader unlitShader(engine.getResourcesDirectory() + "shaders/unlit.vsh",
                       engine.getResourcesDirectory() + "shaders/unlit.fsh");
    engine.addShader("unlit", &unlitShader);

    texture2d crateTex("resources/demo/textures/crate.jpg", GL_RGB);
    engine.addTexture("crate", &crateTex);

    mesh teapotMesh(&objMeshLoader, engine.getResourcesDirectory() + "meshes/teapot.obj");
    engine.addMesh("teapot", &teapotMesh);

    freecam player{&engine};
    engine.setCamera(&player);

    engine.addInitFunction([&player](class engine* e) {
        e->getShader("unlit")->use();
        e->getShader("unlit")->setUniform("ourTexture", 0);
#if DEBUG
        engine::setBackgroundColor(0.0f, 0.0f, 0.3f, 1.0f);
#endif
        e->captureMouse(true);
        player.init(e);
    });
    engine.addRenderFunction([](class engine* e) {
        e->getTexture("crate")->use();
        e->getMesh("teapot")->render(e->getShader("unlit"));
    });
    engine.init();
    engine.run();
}
