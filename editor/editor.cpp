#include "../src/core/engine.h"
#include "../src/sound/oggFileSound.h"
#include "../src/render/phongMaterial.h"
#include "../src/hook/discordRichPresence.h"
#include "../src/resource/provider/filesystemResourceProvider.h"
#include "../src/resource/resourceManager.h"
#include <tinyfiledialogs.h>
#include "../src/ui/markdown.h"
#include "ui/settings.h"
#include "../src/entity/3d/model/mesh3d.h"

using namespace chira;

int main() {
    engine::preInit();
    resourceManager::addResourceProvider("file", new filesystemResourceProvider{"file", "resources/editor"});
    translationManager::addTranslationFile("file://i18n/editor");
    translationManager::addUniversalFile("file://i18n/editor");

    engine::getSettingsLoader()->setValue("engineGui", "discordIntegration", true, false, true);
    bool discordEnabled;
    engine::getSettingsLoader()->getValue("engineGui", "discordIntegration", &discordEnabled);

    engine::addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, []() {
        engine::stop();
    }));
    engine::addKeybind(keybind(GLFW_KEY_1, GLFW_PRESS, []() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }));
    engine::addKeybind(keybind(GLFW_KEY_2, GLFW_PRESS, []() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }));
    engine::addKeybind(keybind(GLFW_KEY_GRAVE_ACCENT, GLFW_PRESS, []() {
        engine::showConsole(!engine::getConsole()->isVisible());
    }));
    engine::addKeybind(keybind(GLFW_KEY_M, GLFW_PRESS, []() {
        engine::getSoundManager()->getSound("helloWorld")->play();
    }));
    engine::addKeybind(keybind(GLFW_KEY_P, GLFW_RELEASE, []() {
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
            logger::log(INFO_IMPORTANT, "File Picker Debug", std::string(path));
            delete path;
        } else {
            logger::log(INFO_IMPORTANT, "File Picker Debug", TR("generic.operation.cancelled"));
        }
    }));

    meshResource* cubeMesh;
    camera3d* camera;

    engine::addInitFunction([&cubeMesh, &camera, &discordEnabled]() {
        if (discordEnabled) {
            discordRichPresence::init(TR("editor.discord.application_id"));
            discordRichPresence::setLargeImage("main_logo");
            discordRichPresence::setState("https://discord.gg/ASgHFkX");
        }

        // Don't release the fontResource when done to keep it cached
        auto* noto = resourceManager::getResource<fontResource>("file://fonts/default.json");
        ImGui::GetIO().FontDefault = noto->getFont();

        auto* cubeMaterial = resourceManager::getResource<phongMaterial>("file://materials/cubeMaterial.json");
        cubeMesh = resourceManager::getResource<meshResource>("file://meshes/teapot.json", cubeMaterial);

        engine::getRoot()->addChild(new mesh3d{cubeMesh});
        //        new bulletRigidBodyComponent{"file://physics/cube_dynamic.json", glm::vec3{0, 5, -10}}
        /*
        componentManager::getWorld<extensibleWorld>(worldId)->add((new propBulletPhysicsEntity{})->init(
                new meshComponent{cubeMesh},
                new bulletRigidBodyComponent{"file://physics/ground_static.json", glm::vec3{3, -5, -13}}));

        auto* tex = resourceManager::getResource<texture>("file://textures/ui/icon.json");
        componentManager::getWorld<extensibleWorld>(worldId)->add(
                new extensibleUiWindowComponent{TR("debug.imgui.texture_test"), true, [tex](double delta) {
                    ImGui::Text("size = %d x %d", 512, 512);
                    ImGui::Image((void*) (intptr_t) tex->getHandle(), ImVec2(512, 512));
                }});
        */

        auto* settingsUi = new settings{false};
        engine::addKeybind(keybind(GLFW_KEY_O, GLFW_PRESS, [settingsUi](){
            settingsUi->setVisible(!settingsUi->isVisible());
        }));

        engine::captureMouse(true);
        camera = new camera3d{"freecam", PERSPECTIVE};
        engine::getRoot()->addChild(camera);
        engine::getRoot()->setMainCamera(camera);

        engine::getAngelscriptProvider()->addScript("file://scripts/testScript.as");

        auto* sound = new oggFileSound();
        sound->init("helloWorldCutMono.ogg");
        engine::getSoundManager()->addSound("helloWorld", sound);

        cubeMaterial->setShininess();
        cubeMaterial->setLambertFactor();
        shader* cubeShader = cubeMaterial->getShader();
        cubeShader->use();
        cubeShader->setUniform("light.ambient", 0.1f, 0.1f, 0.1f);
        cubeShader->setUniform("light.diffuse", 1.0f, 1.0f, 1.0f);
        cubeShader->setUniform("light.specular", 1.0f, 1.0f, 1.0f);
        cubeShader->setUniform("light.position", 0.0f, 5.0f, 0.0f);

        engine::getRoot()->setSkybox("file://materials/skyboxShanghaiMaterial.json");
    });
    engine::init();

    engine::addRenderFunction([cubeMesh, camera]() {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);

        // todo: use UBO
        cubeMesh->getMaterial()->getShader()->use();
        cubeMesh->getMaterial()->getShader()->setUniform("p", camera->getProjection());
        cubeMesh->getMaterial()->getShader()->setUniform("v", camera->getView());

        // todo: use UBO
        engine::getRoot()->getSkybox()->getShader()->use();
        engine::getRoot()->getSkybox()->getShader()->setUniform("p", camera->getProjection());
        auto view = glm::mat4(glm::mat3(camera->getView()));
        engine::getRoot()->getSkybox()->getShader()->setUniform("v", &view);
    });
    engine::run();
}
