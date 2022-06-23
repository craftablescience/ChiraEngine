// Disable console window on Windows (MSVC)
#include <utility/Platform.h>
#if defined(_WIN32) && !defined(DEBUG) && defined(CHIRA_COMPILER_MSVC)
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <fstream>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <entity/model/Mesh.h>
#include <entity/model/MeshDynamic.h>
#include <entity/camera/EditorCamera.h>

using namespace chira;

int main() {
    Engine::preInit("editor_config.json");
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    Engine::init();

    Engine::getWindow()->setBackgroundColor(ColorRGB{0.15f});

    auto camera = new EditorCamera{CameraProjectionMode::PERSPECTIVE, 120.f};
    camera->translate({-6.f * sqrtf(3.f), 6, 0});
    camera->setPitch(30.f);
    camera->setYaw(270.f);
    Engine::getWindow()->addChild(camera);
    Engine::getWindow()->setCamera(camera);
    EditorCamera::setupKeybinds();

    auto grid = new MeshDynamic{"grid"};
    auto gridMesh = grid->getMesh();
    gridMesh->setMaterial(CHIRA_GET_MATERIAL("MaterialUntextured", "file://materials/unlit.json"));
    for (int i = -5; i <= 5; i++) {
        gridMesh->addCube(Vertex{{i, 0, 0}}, {0.025f, 0.025f, 10.f});
        gridMesh->addCube(Vertex{{0, 0, i}}, {10.f, 0.025f, 0.025f});
    }
    gridMesh->addCube({{2.5f, 0, 0}, {0, 0, 0}, {1, 0, 0}}, {5.f + 0.026f, 0.03f, 0.03f});
    gridMesh->addCube({{0, 0, 2.5f}, {0, 0, 0}, {0, 0, 1}}, {0.03f, 0.03f, 5.f + 0.026f});
    gridMesh->addCube({{0, 0,    0}, {0, 0, 0}, {0, 1, 0}}, glm::vec3{0.05f});
    Engine::getWindow()->addChild(grid);

    Engine::run();
}
