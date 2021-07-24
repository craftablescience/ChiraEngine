#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/objMeshLoader.h"
#include "../src/render/freecam.h"
#include "../src/sound/oggFileStream.h"

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
    engine.addKeybind(keybind(GLFW_KEY_M, GLFW_PRESS, [](class engine* e) {
        e->getSoundManager()->getSound("helloWorld")->play();
    }));

    engine.addShader("unlit", new shader(
            engine.getResourcesDirectory() + "shaders/unlit.vsh",
            engine.getResourcesDirectory() + "shaders/unlit.fsh"));
    engine.addTexture("crate", new texture2d(
            "resources/demo/textures/crate.jpg", GL_RGB));
    engine.addMesh("teapot", new mesh(
            &objMeshLoader,
            engine.getResourcesDirectory() + "meshes/teapot.obj"));

    freecam player{&engine};
    engine.setCamera(&player);

    engine.addInitFunction([&player](class engine* e) {
        bool angelscriptEnabled = true;
        e->getSettingsLoader()->getValue("scripting", "angelscript", &angelscriptEnabled);
        if (angelscriptEnabled) {
            ((angelscriptProvider*) e->getScriptProvider("angelscript"))->addScript(
                    new angelscriptHolder{"resources/demo/scripts/testScript.as"});
        }

        auto* sound = new oggFileStream();
        sound->init("resources/demo/sounds/helloWorldCut.ogg");
        e->getSoundManager()->addSound("helloWorld", sound);

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
