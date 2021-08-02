#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/objMeshLoader.h"
#include "../src/render/freecam.h"
#include "../src/sound/oggFileSound.h"
#include "../src/render/phongMaterial.h"

int main() {
    engine engine;
    virtualFileSystem::addResourceDirectory("resources/demo/");

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

    engine::addTexture("container_diffuse", new texture2d("container_diffuse.png", GL_RGBA));
    engine::addTexture("container_specular", new texture2d("container_specular.png", GL_RGBA));
    engine::addShader("phonglit", new shader("phonglit.vsh", "phonglit.fsh"));
    engine::addMaterial("phonglit", new phongMaterial{"phonglit", "container_diffuse", "container_specular"});
    engine::addMesh("cube", new mesh(&objMeshLoader, "teapot.obj", "phonglit"));

    engine.addInitFunction([](class engine* e) {
        e->captureMouse(true);
        e->setWorld(new world{e, new freecam{e}});
        e->getWorld()->addMesh("cube");

        bool angelscriptEnabled = true;
        e->getSettingsLoader()->getValue("scripting", "angelscript", &angelscriptEnabled);
        if (angelscriptEnabled) {
            ((angelscriptProvider*) e->getScriptProvider("angelscript"))->addScript(new angelscriptHolder{"testScript.as"});
        }

        auto* sound = new oggFileSound();
        sound->init("helloWorldCutMono.ogg");
        e->getSoundManager()->addSound("helloWorld", sound);

        ((phongMaterial*) engine::getMaterial("phonglit"))->setShininess();
        ((phongMaterial*) engine::getMaterial("phonglit"))->setLambertFactor();
        engine::getShader("phonglit")->use();
        engine::getShader("phonglit")->setUniform("light.ambient", 0.1f, 0.1f, 0.1f);
        engine::getShader("phonglit")->setUniform("light.diffuse", 1.0f, 1.0f, 1.0f);
        engine::getShader("phonglit")->setUniform("light.specular", 1.0f, 1.0f, 1.0f);
        engine::getShader("phonglit")->setUniform("light.position", 0.0f, 5.0f, 0.0f);

#if DEBUG
        engine::setBackgroundColor(0.0f, 0.0f, 0.3f, 1.0f);
#endif
    });

    engine.init();
    engine.run();
}
