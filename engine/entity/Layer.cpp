#include "Layer.h"

#include <core/Assertions.h>
#include <render/shader/UBO.h>
#include "component/AngelScriptComponent.h"
#include "component/AudioSpeechComponent.h"
#include "component/BillboardComponent.h"
#include "component/MeshComponent.h"
#include "component/MeshDynamicComponent.h"
#include "component/SkyboxComponent.h"
#include "component/TagComponents.h"
#include "component/TransformComponent.h"
#include "component/MeshSpriteComponent.h"

using namespace chira;

Layer::Layer(glm::vec2i size_, ColorRGB backgroundColor_, bool linearFiltering_)
        : size(size_)
        , backgroundColor(backgroundColor_)
        , linearFiltering(linearFiltering_) {
    this->recreateFrameBuffer();
}

Scene* Layer::addScene() {
    auto uuid = UUIDGenerator::getNewUUID();
    return this->addScene(uuid);
}

Scene* Layer::addScene(const std::string& name) {
    auto* scene = this->addScene();
    scene->getRegistry().emplace<NameComponent>(scene->getRawHandle(), name);
    return scene;
}

Scene* Layer::addScene(uuids::uuid uuid) {
    runtime_assert(!this->scenes.contains(uuid), "Scene UUID is already present!");
    this->scenes[uuid] = std::unique_ptr<Scene>(new Scene{});
    auto& scene = this->scenes.at(uuid);
    scene->getRegistry().get<UUIDComponent>(scene->getRawHandle()).uuid = uuid;
    return scene.get();
}

Scene* Layer::addScene(uuids::uuid uuid, const std::string& name) {
    auto* scene = this->addScene(uuid);
    scene->getRegistry().emplace<NameComponent>(scene->getRawHandle(), name);
    return scene;
}

Scene* Layer::getScene(uuids::uuid sceneID) {
    if (this->hasScene(sceneID)) {
        return nullptr;
    }
    return this->scenes.at(sceneID).get();
}

bool Layer::hasScene(uuids::uuid sceneID) {
    return this->scenes.contains(sceneID);
}

void Layer::removeScene(uuids::uuid sceneID) {
    runtime_assert(this->hasScene(sceneID), "Trying to remove a scene with a UUID that doesn't exist!");
    this->scenes.erase(sceneID);
}

void Layer::removeAllScenes() {
    for (const auto& [uuid, scene] : this->scenes) {
        this->removeScene(uuid);
    }
}

void Layer::update() {
    for (const auto& [uuid, scene] : this->scenes) {
        if (auto* camera = scene->getCamera()) {
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

        // Update AngelScriptComponent
        auto angelScriptView = scene->getEntities<AngelScriptComponent>();
        for (const auto [entity, angelScriptComponent] : angelScriptView.each()) {
            angelScriptComponent.update();
        }
    }
}

void Layer::render() {
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

    // Render scenes
    for (const auto& [uuid, scene] : this->scenes) {
        auto& registry = scene->getRegistry();

        // Set up camera
        scene->setupForRender(this->size);

        // Render MeshComponent
        auto meshView = scene->getEntities<MeshComponent>(entt::exclude<NoRenderTagComponent>);
        for (auto entity : meshView) {
            auto& transformComponent = registry.get<TransformComponent>(entity);
            auto& meshComponent = registry.get<MeshComponent>(entity);
            meshComponent.getMeshResource()->render(transformComponent.getMatrix());
        }

        // Render MeshDynamicComponent
        auto meshDynamicView = scene->getEntities<MeshDynamicComponent>(entt::exclude<NoRenderTagComponent>);
        for (auto entity : meshDynamicView) {
            auto& transformComponent = registry.get<TransformComponent>(entity);
            auto& meshDynamicComponent = registry.get<MeshDynamicComponent>(entity);
            meshDynamicComponent.meshBuilder.render(transformComponent.getMatrix());
        }

        // Render MeshSpriteComponent
        auto meshSpriteView = scene->getEntities<MeshSpriteComponent>(entt::exclude<NoRenderTagComponent>);
        for (auto entity : meshSpriteView) {
            auto& transformComponent = registry.get<TransformComponent>(entity);
            auto& meshSpriteComponent = registry.get<MeshSpriteComponent>(entity);
            meshSpriteComponent.sprite.render(transformComponent.getMatrix());
        }

        // Render SkyboxComponent
        auto skyboxView = registry.view<SkyboxComponent>();
        if (!skyboxView.empty()) {
            auto& skyboxComponent = skyboxView.get<SkyboxComponent>(skyboxView.front());
            skyboxComponent.skybox.render(glm::identity<glm::mat4>());
        }

        // Render AngelScriptComponent
        auto angelScriptView = scene->getEntities<AngelScriptComponent>(entt::exclude<NoRenderTagComponent>);
        for (const auto [entity, angelScriptComponent] : angelScriptView.each()) {
            angelScriptComponent.render();
        }
    }
    Renderer::popFrameBuffer();
}

void Layer::recreateFrameBuffer() {
    if (this->frameBufferHandle) {
        Renderer::destroyFrameBuffer(this->frameBufferHandle);
    }
    this->frameBufferHandle = Renderer::createFrameBuffer(this->size.x, this->size.y, WrapMode::REPEAT, WrapMode::REPEAT,
                                                          this->linearFiltering ? FilterMode::LINEAR : FilterMode::NEAREST, true);
}
