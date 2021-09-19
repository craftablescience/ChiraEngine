#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/objMeshLoader.h"
#include "../src/render/freecam.h"
#include "../src/sound/oggFileSound.h"
#include "../src/render/phongMaterial.h"
#include "../src/implementation/discordRichPresence.h"
#include "../src/resource/filesystemResourceProvider.h"
#include "../src/resource/resourceManager.h"
#include "tinyfiledialogs.h"
#include "../src/ui/markdown.h"

int main() {
    engine engine;
    resourceManager::addResourceProvider("file", new filesystemResourceProvider{"file", "resources/editor"});
    chira::translationManager::addTranslationFile("file://i18n/editor");

    engine::getSettingsLoader()->setValue("engineGui", "discordIntegration", true, false, true);
    bool discordEnabled;
    engine::getSettingsLoader()->getValue("engineGui", "discordIntegration", &discordEnabled);

    mesh::addMeshLoader("obj", new objMeshLoader{});

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
    engine.addKeybind(keybind(GLFW_KEY_B, GLFW_RELEASE, [](class engine* e) {
        const char* path = tinyfd_openFileDialog(
                TR("ui.window.select_file").c_str(),
#if WIN32
                "C:\\",
#else
                "/",
#endif
                0,
                nullptr,
                nullptr,
                0
        );
        if (path) {
            chira::logger::log(INFO_IMPORTANT, "File Picker Debug", std::string(path));
            delete path;
        } else {
            chira::logger::log(INFO_IMPORTANT, "File Picker Debug", TR("generic.operation.cancelled"));
        }
    }));

    mesh* cubeMesh;

    engine.addInitFunction([&cubeMesh, &discordEnabled](class engine* e) {
        ImGuiIO& io = ImGui::GetIO();

        // Don't release the fontResource when done to keep it cached
        auto* noto = resourceManager::getResource<fontResource>("file://fonts/default.json");
        io.FontDefault = noto->getFont();

        auto* cubeMaterial = resourceManager::getResource<phongMaterial>("file://materials/cubeMaterial.json");
        cubeMesh = resourceManager::getResource<mesh>("file://meshes/teapot.json", cubeMaterial);

        if (discordEnabled) {
            discordRichPresence::init("875778280899358720");
            discordRichPresence::setLargeImage("main_logo");
            discordRichPresence::setDetails(TR("ui.window.title"));
            discordRichPresence::setState("https://discord.gg/ASgHFkX");
        }

        e->captureMouse(true);
        e->setMainCamera(new freecam{e});

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

    auto* tex = resourceManager::getResource<texture2d>("file://textures/ui/icon.png", GL_RGBA, false);
    engine.addRenderFunction([tex, cubeMesh](class engine* e) {
        cubeMesh->getMaterial()->getShader()->setUniform("p", e->getMainCamera()->getProjectionMatrix());
        cubeMesh->getMaterial()->getShader()->setUniform("v", e->getMainCamera()->getViewMatrix());
        cubeMesh->render();

        ImGui::Begin(TR("debug.imgui.texture_test").c_str());
        ImGui::Text("size = %d x %d", 512, 512);
        ImGui::Image((void*)(intptr_t) tex->getHandle(), ImVec2(512, 512));
        markdown::create("Hello -- [https://belewis.me](https://belewis.me)");
        ImGui::End();
    });
    engine.run();
}
