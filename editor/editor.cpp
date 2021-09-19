#include "../src/core/engine.h"
#include "../src/render/texture2d.h"
#include "../src/loader/objMeshLoader.h"
#include "../src/render/freecam.h"
#include "../src/sound/oggFileSound.h"
#include "../src/render/phongMaterial.h"
#include "../src/implementation/discordRichPresence.h"
#include "../src/resource/filesystemResourceProvider.h"
#include "../src/resource/resourceManager.h"

int main() {
    engine engine;
    resourceManager::addResourceProvider("file", new filesystemResourceProvider{"file", "resources/editor"});
    chira::translationManager::addTranslationFile("editor");

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

    mesh* cubeMesh;

    engine.addInitFunction([&cubeMesh, &discordEnabled](class engine* e) {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        std::string filepath = ((filesystemResourceProvider*) resourceManager::getResourceProviderWithResource("file", "fonts/noto_sans_jp/NotoSansJP-Regular.otf"))->getPath() + "/fonts/noto_sans_jp/NotoSansJP-Regular.otf";
        ImFont* font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        if (font != nullptr) {
            io.FontDefault = font;
        } else {
            io.Fonts->AddFontDefault();
        }
        io.Fonts->Build();

        auto* cubeMaterial = resourceManager::getResource<phongMaterial>("file", "materials/cubeMaterial.json");
        cubeMesh = resourceManager::getResource<mesh>("file", "meshes/teapot.json", cubeMaterial);

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

    auto* tex = resourceManager::getResource<texture2d>("file", "textures/ui/icon.png", GL_RGBA, false);
    engine.addRenderFunction([tex, cubeMesh](class engine* e) {
        cubeMesh->getMaterial()->getShader()->setUniform("p", e->getMainCamera()->getProjectionMatrix());
        cubeMesh->getMaterial()->getShader()->setUniform("v", e->getMainCamera()->getViewMatrix());
        cubeMesh->render();

        ImGui::Begin(TR("debug.imgui.texture_test").c_str());
        ImGui::Text("size = %d x %d", 512, 512);
        ImGui::Image((void*)(intptr_t) tex->getHandle(), ImVec2(512, 512));
        ImGui::End();
    });
    engine.run();
}
