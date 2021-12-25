#include <core/engine.h>
#include <sound/oggFileSound.h>
#include <render/material/phongMaterial.h>
#include <hook/discordRichPresence.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <entity/3d/model/mesh3d.h>
#include <i18n/translationManager.h>
#include <entity/3d/physics/bulletRigidBody.h>
#include <entity/imgui/settings/settings.h>
#include <entity/3d/camera/freecam.h>
#include <entity/imgui/console/console.h>
#include <entity/imgui/profiler/profiler.h>

using namespace chira;

int main() {
    engine::preInit("settings_editor.json");
    resource::addResourceProvider(new filesystemResourceProvider{"editor"});
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
        engine::getConsole()->setVisible(!engine::getConsole()->isVisible());
    }));
#ifdef DEBUG
    engine::addKeybind(keybind(GLFW_KEY_F1, GLFW_PRESS, []() {
        engine::getProfiler()->setVisible(!engine::getProfiler()->isVisible());
    }));
#endif
    engine::addKeybind(keybind(GLFW_KEY_M, GLFW_PRESS, []() {
        engine::getSoundManager()->getSound("helloWorld")->play();
    }));

    engine::addInitFunction([&discordEnabled]() {
        //region Enable Discord Rich Presence
        if (discordEnabled) {
            discordRichPresence::init(TR("editor.discord.application_id"));
            discordRichPresence::setLargeImage("main_logo");
            discordRichPresence::setState("https://discord.gg/ASgHFkX");
        }
        //endregion

        //region Add a teapot with a static rigidbody
        auto staticTeapot = new bulletRigidBody{"static", "file://physics/ground_static.json"};
        staticTeapot->translate(glm::vec3{3,0,-13});
        auto cubeMesh = resource::getResource<meshResource>("file://meshes/teapot.json");
        staticTeapot->addChild(new mesh3d{"teapotMesh", cubeMesh});

        auto fallingTeapot = new bulletRigidBody{"file://physics/cube_dynamic.json"};
        fallingTeapot->translate(glm::vec3{0,15,-10});
        fallingTeapot->addChild(new mesh3d{cubeMesh});

        engine::getRoot()->addChild(staticTeapot);
        engine::getRoot()->addChild(fallingTeapot);
        //endregion

        //region Add the settings UI window
        auto settingsUI = new settings{};
        engine::getRoot()->addChild(settingsUI);
        engine::addKeybind(keybind(GLFW_KEY_O, GLFW_PRESS, [settingsUI](){
            settingsUI->setVisible(!settingsUI->isVisible());
        }));
        //endregion

        //region Add the camera
        engine::captureMouse(true);
        auto camera = new freecam{cameraProjectionMode::PERSPECTIVE};
        engine::getRoot()->addChild(camera);
        engine::getRoot()->setMainCamera(camera);
        camera->translate(glm::vec3{0,0,15});
        //endregion

        //region Add a test script
        engine::getAngelscriptProvider()->addScript("file://scripts/testScript.as");
        //endregion

        //region Add a test sound
        auto* sound = new oggFileSound();
        sound->init("helloWorldCutMono.ogg");
        engine::getSoundManager()->addSound("helloWorld", sound);
        //endregion

        //region Apply some lighting properties to the mesh
        auto teapotMaterial = cubeMesh->getMaterial().castReinterpret<phongMaterial>();
        teapotMaterial->setShininess();
        teapotMaterial->setLambertFactor();
        auto teapotShader = teapotMaterial->getShader();
        teapotShader->use();
        teapotShader->setUniform("light.ambient", 0.1f, 0.1f, 0.1f);
        teapotShader->setUniform("light.diffuse", 1.0f, 1.0f, 1.0f);
        teapotShader->setUniform("light.specular", 1.0f, 1.0f, 1.0f);
        teapotShader->setUniform("light.position", 0.0f, 5.0f, 0.0f);
        //endregion

        //region Set a nice skybox
        engine::getRoot()->setSkybox("file://materials/skybox/shanghai.json");
        //endregion
    });
    engine::init();

    engine::addRenderFunction([]() {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);
    });
    engine::run();
}
