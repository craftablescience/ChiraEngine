#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/objMeshLoader.h"
#include "../src/render/freecam.h"
#include "../src/sound/oggFileSound.h"
#include "../src/render/phongMaterial.h"
#include "../src/implementation/discordRichPresence.h"
#include "../src/resource/resourceManager.h"
#include "../src/resource/filesystemResourceProvider.h"

int main() {
    engine engine;
    resourceManager::addResourceProvider("filesystem", new filesystemResourceProvider{"filesystem", "resources/engine"});
    resourceManager::addResourceProvider("filesystem", new filesystemResourceProvider{"filesystem", "resources/demo"});
    virtualFileSystem::addResourceDirectory(ENGINE_FILESYSTEM_PREFIX);
    virtualFileSystem::addResourceDirectory("resources/demo/");

    mesh::addMeshLoader("obj", new objMeshLoader{});

    engine::getSettingsLoader()->setValue("engine", "title", std::string("Demo Window"), true, true);

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

    engine.addInitFunction([](class engine* e) {
        auto* cubeMaterial = resourceManager::getResource<phongMaterial>("filesystem", "materials/cubeMaterial.json");
        auto* cubeMesh = resourceManager::getResource<mesh>("filesystem", "meshes/cube.json", cubeMaterial);

        discordRichPresence::init("875778280899358720");
        discordRichPresence::setLargeImage("main_logo");
        discordRichPresence::setDetails("Demo App Running");
        discordRichPresence::setState("https://discord.gg/ASgHFkX");

        e->captureMouse(true);
        e->setWorld(new world{e, new freecam{e}});
        e->getWorld()->addMesh(cubeMesh);

        bool angelscriptEnabled = true;
        engine::getSettingsLoader()->getValue("scripting", "angelscript", &angelscriptEnabled);
        if (angelscriptEnabled) {
            ((angelscriptProvider*) e->getScriptProvider("angelscript"))->addScript(new angelscriptHolder{"testScript.as"});
        }

        auto* sound = new oggFileSound();
        sound->init("helloWorldCutMono.ogg");
        e->getSoundManager()->addSound("helloWorld", sound);

        cubeMaterial->setShininess();
        cubeMaterial->setLambertFactor();
        shader* cubeShader = cubeMaterial->getShader();
        cubeShader->use();
        cubeShader->setUniform("light.ambient", 0.1f, 0.1f, 0.1f);
        cubeShader->setUniform("light.diffuse", 1.0f, 1.0f, 1.0f);
        cubeShader->setUniform("light.specular", 1.0f, 1.0f, 1.0f);
        cubeShader->setUniform("light.position", 0.0f, 5.0f, 0.0f);

#if DEBUG
        engine::setBackgroundColor(0.0f, 0.0f, 0.3f, 1.0f);
#endif
    });
    engine.init();
    engine.run();
}
