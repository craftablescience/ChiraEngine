#include <core/engine.h>
#include <input/inputManager.h>
#include <sound/oggFileSound.h>
#include <hook/discordRPC.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <i18n/translationManager.h>
#include <entity/model/mesh.h>
#include <entity/physics/bulletRigidBody.h>
#include <entity/camera/editorCamera.h>
#include <entity/gui/console.h>
#include <entity/gui/profiler.h>
#include <entity/gui/settings.h>
#if defined(CHIRA_BUILD_WITH_STEAMWORKS) && defined(DEBUG)
#include <hook/steamAPI.h>
#endif

using namespace chira;

int main() {
    Engine::preInit("settings_editor.json");
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    TranslationManager::addUniversalFile("file://i18n/editor");

    Engine::getSettingsLoader()->setValue("engineGui", "discordIntegration", true, false, true);
    bool discordEnabled;
    Engine::getSettingsLoader()->getValue("engineGui", "discordIntegration", &discordEnabled);

#ifdef CHIRA_BUILD_WITH_STEAMWORKS
    Engine::getSettingsLoader()->setValue("engine", "steamworks", true, true, true);
#ifdef DEBUG
    SteamAPI::generateAppIDFile(1728950);
#endif
#endif

    InputManager::addCallback(InputKeyButton{Key::ESCAPE, InputKeyEventType::PRESSED, []{
        Engine::stop();
    }});
    InputManager::addCallback(InputKeyButton{Key::ONE, InputKeyEventType::PRESSED, []{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }});
    InputManager::addCallback(InputKeyButton{Key::TWO, InputKeyEventType::PRESSED, []{
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }});
    InputManager::addCallback(InputKeyButton{Key::GRAVE_ACCENT, InputKeyEventType::PRESSED, []{
        Engine::getConsole()->setVisible(!Engine::getConsole()->isVisible());
    }});
#ifdef DEBUG
    InputManager::addCallback(InputKeyButton{Key::F1, InputKeyEventType::PRESSED, []{
        Engine::getProfiler()->setVisible(!Engine::getProfiler()->isVisible());
    }});
#endif
    InputManager::addCallback(InputKeyButton{Key::M, InputKeyEventType::PRESSED, []{
        Engine::getSoundManager()->getSound("helloWorld")->play();
    }});

    Engine::addInitFunction([&discordEnabled]{
        //region Enable Discord Rich Presence
        if (discordEnabled) {
            DiscordRPC::init(TR("editor.discord.application_id"));
            DiscordRPC::setLargeImage("main_logo");
            DiscordRPC::setState("https://discord.gg/ASgHFkX");
        }
        //endregion

        //region Add a teapot with a static rigidbody
        auto staticTeapot = new BulletRigidBody{"file://physics/ground_static.json"};
        staticTeapot->translate(glm::vec3{3,0,-13});
        staticTeapot->addChild(new Mesh{"file://meshes/teapot.json"});
        Engine::getWindow()->addChild(staticTeapot);

        auto fallingTeapot = new BulletRigidBody{"file://physics/cube_dynamic.json"};
        fallingTeapot->translate(glm::vec3{0,15,-10});
        fallingTeapot->addChild(new Mesh{"file://meshes/teapot.json"});
        Engine::getWindow()->addChild(fallingTeapot);
        //endregion

        //region Add the settings UI window
        auto settingsUI = new Settings{};
        Engine::getWindow()->addChild(settingsUI);
        InputManager::addCallback(InputKeyButton{Key::O, InputKeyEventType::PRESSED, [settingsUI]{
            settingsUI->setVisible(!settingsUI->isVisible());
        }});
        //endregion

        //region Add the camera
        auto camera = new EditorCamera{CameraProjectionMode::PERSPECTIVE};
        Engine::getWindow()->addChild(camera);
        Engine::getWindow()->setCamera(camera);
        EditorCamera::setupKeybinds();
        camera->translate(glm::vec3{0,0,15});
        //endregion

        //region Add a test script
        Engine::getAngelscriptProvider()->addScript("file://scripts/testScript.as");
        //endregion

        //region Add a test sound
        auto sound = new OGGFileSound();
        sound->init("file://sounds/helloWorldCutMono.ogg");
        Engine::getSoundManager()->addSound("helloWorld", sound);
        //endregion

        //region Apply some lighting properties to the mesh
        auto teapotMesh = Resource::getResource<MeshDataResource>("file://meshes/teapot.json");
        const auto teapotShader = teapotMesh->getMaterial()->getShader();
        teapotShader->use();
        teapotShader->setUniform("light.ambient", 0.1f, 0.1f, 0.1f);
        teapotShader->setUniform("light.diffuse", 1.0f, 1.0f, 1.0f);
        teapotShader->setUniform("light.specular", 1.0f, 1.0f, 1.0f);
        teapotShader->setUniform("light.position", 0.0f, 5.0f, 0.0f);
        //endregion

        //region Set a nice skybox
        Engine::getWindow()->setSkybox("file://materials/skybox/shanghai.json");
        //endregion
    });
    Engine::init();

    Engine::addRenderFunction([]{
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);
    });
    Engine::run();
}
