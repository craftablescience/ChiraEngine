#include "Viewport.h"

#include <core/debug/Assertions.h>
#include <core/utility/Types.h>
#include <render/shader/UBO.h>
#include "component/AudioSpeechComponent.h"
#include "component/BillboardComponent.h"
#include "component/MeshComponent.h"
#include "component/MeshDynamicComponent.h"
#include "component/SkyboxComponent.h"
#include "component/TagComponents.h"
#include "component/TransformComponent.h"
#include "component/MeshSpriteComponent.h"
#include "component/LayerComponents.h"

using namespace chira;

Viewport::Viewport(glm::vec2i size_, ColorRGB backgroundColor_, bool linearFiltering_)
        : size(size_)
        , backgroundColor(backgroundColor_)
        , linearFiltering(linearFiltering_) {
    this->recreateFrameBuffer();
}

Scene* Viewport::addScene() {
    auto uuid = UUIDGenerator::getNewUUID();
    return this->addScene(uuid);
}

Scene* Viewport::addScene(const std::string& name) {
    auto* scene = this->addScene();
    scene->getRegistry().emplace<NameComponent>(scene->getRawHandle(), name);
    return scene;
}

Scene* Viewport::addScene(uuids::uuid uuid) {
    chiraAssert(!this->scenes.contains(uuid), "Scene UUID is already present!");
    this->scenes[uuid] = std::unique_ptr<Scene>(new Scene{});
    auto& scene = this->scenes.at(uuid);
    scene->getRegistry().get<UUIDComponent>(scene->getRawHandle()).uuid = uuid;
    return scene.get();
}

Scene* Viewport::addScene(uuids::uuid uuid, const std::string& name) {
    auto* scene = this->addScene(uuid);
    scene->getRegistry().emplace<NameComponent>(scene->getRawHandle(), name);
    return scene;
}

Scene* Viewport::getScene(uuids::uuid sceneID) {
    if (this->hasScene(sceneID)) {
        return nullptr;
    }
    return this->scenes.at(sceneID).get();
}

bool Viewport::hasScene(uuids::uuid sceneID) {
    return this->scenes.contains(sceneID);
}

void Viewport::removeScene(uuids::uuid sceneID) {
    chiraAssert(this->hasScene(sceneID), "Trying to remove a scene with a UUID that doesn't exist!");
    this->scenes.erase(sceneID);
}

void Viewport::removeAllScenes() {
    for (const auto& [uuid, scene] : this->scenes) {
        this->removeScene(uuid);
    }
}

void Viewport::update() {
    for (const auto& [uuid, scene] : this->scenes) {
        if (auto* camera = this->getCamera()) {
            // Update BillboardComponent
            auto billboardView = scene->getEntities<BillboardComponent>();
            for (const auto [entity, billboardComponent] : billboardView.each()) {
                if (billboardComponent.x)
                    billboardComponent.transform->setPitch(camera->transform->getPitch());
                if (billboardComponent.y)
                    billboardComponent.transform->setYaw(camera->transform->getYaw());
                if (billboardComponent.z)
                    billboardComponent.transform->setRoll(camera->transform->getRoll());
            }
        }

#if 0
        // Update AngelScriptComponent
        auto angelScriptView = scene->getEntities<AngelScriptComponent>();
        for (const auto [entity, angelScriptComponent] : angelScriptView.each()) {
            angelScriptComponent.update();
        }
#endif
    }
}

void Viewport::render() {
    Renderer::setClearColor({this->backgroundColor, 1.f});
    Renderer::pushFrameBuffer(this->frameBufferHandle);

    // Set up lighting
    DirectionalLightComponent* directionalLightComponentArray[DIRECTIONAL_LIGHT_MAX] {nullptr};
    int directionalLightsCount = 0;
    PointLightComponent* pointLightComponentArray[POINT_LIGHT_MAX] {nullptr};
    int pointLightsCount = 0;
    SpotLightComponent* spotLightComponentArray[SPOT_LIGHT_MAX] {nullptr};
    int spotLightsCount = 0;

    for (const auto& [uuid, scene] : this->scenes) {
        auto directionalLightsView = scene->getRegistry().view<DirectionalLightComponent>();
        for (auto [entity, directionalLightComponent] : directionalLightsView.each()) {
            if (directionalLightsCount < DIRECTIONAL_LIGHT_MAX) {
                directionalLightComponentArray[directionalLightsCount] = &directionalLightComponent;
            }
            directionalLightsCount++;
        }

        auto pointLightsView = scene->getRegistry().view<PointLightComponent>();
        for (auto [entity, pointLightComponent] : pointLightsView.each()) {
            if (pointLightsCount < POINT_LIGHT_MAX) {
                pointLightComponentArray[pointLightsCount] = &pointLightComponent;
            }
            pointLightsCount++;
        }

        auto spotLightsView = scene->getRegistry().view<SpotLightComponent>();
        for (auto [entity, spotLightComponent] : spotLightsView.each()) {
            if (spotLightsCount < SPOT_LIGHT_MAX) {
                spotLightComponentArray[spotLightsCount] = &spotLightComponent;
            }
            spotLightsCount++;
        }
    }
    LightsUBO::get().update(directionalLightComponentArray, pointLightComponentArray, spotLightComponentArray,
                            {directionalLightsCount, pointLightsCount, spotLightsCount});

    foreach(LAYER_COMPONENTS, [&](auto layer) {
        if (!(this->getCamera()->activeLayers & layer.index)) {
            return;
        }

        using CurrentLayer = decltype(layer);
        for (const auto& [uuid, scene] : this->scenes) {
            auto& registry = scene->getRegistry();

            // Set up camera
            scene->setupForRender(this->size);

            auto meshView = scene->template getEntities<MeshComponent, CurrentLayer>(entt::exclude<NoRenderTagComponent>);
            for (auto entity : meshView) {
                auto& transformComponent = registry.template get<TransformComponent>(entity);
                auto& meshComponent = registry.template get<MeshComponent>(entity);
                meshComponent.mesh->render(transformComponent.getMatrix());
            }

            // Render MeshDynamicComponent
            auto meshDynamicView = scene->template getEntities<MeshDynamicComponent, CurrentLayer>(entt::exclude<NoRenderTagComponent>);
            for (auto entity : meshDynamicView) {
                auto& transformComponent = registry.template get<TransformComponent>(entity);
                auto& meshDynamicComponent = registry.template get<MeshDynamicComponent>(entity);
                meshDynamicComponent.meshBuilder.render(transformComponent.getMatrix());
            }

            // Render MeshSpriteComponent
            auto meshSpriteView = scene->template getEntities<MeshSpriteComponent, CurrentLayer>(entt::exclude<NoRenderTagComponent>);
            for (auto entity : meshSpriteView) {
                auto& transformComponent = registry.template get<TransformComponent>(entity);
                auto& meshSpriteComponent = registry.template get<MeshSpriteComponent>(entity);
                meshSpriteComponent.sprite.render(transformComponent.getMatrix());
            }
        }
    });

    // Render scenes
    for (const auto& [uuid, scene] : this->scenes) {
        auto& registry = scene->getRegistry();

        // Render SkyboxComponent
        auto skyboxView = registry.view<SkyboxComponent>();
        if (!skyboxView.empty()) {
            auto& skyboxComponent = skyboxView.get<SkyboxComponent>(skyboxView.front());
            skyboxComponent.skybox.render(glm::identity<glm::mat4>());
        }
    }
    Renderer::popFrameBuffer();
}

void Viewport::recreateFrameBuffer() {
    if (this->frameBufferHandle) {
        Renderer::destroyFrameBuffer(this->frameBufferHandle);
    }
    this->frameBufferHandle = Renderer::createFrameBuffer(this->size.x, this->size.y, WrapMode::REPEAT, WrapMode::REPEAT,
                                                          this->linearFiltering ? FilterMode::LINEAR : FilterMode::NEAREST, true);
}
