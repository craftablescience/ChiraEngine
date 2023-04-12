#include "Scene.h"

#include <core/Assertions.h>
#include <render/shader/UBO.h>
#include "component/AngelScriptComponent.h"
#include "component/CameraComponent.h"
#include "component/LightComponents.h"
#include "component/TagComponents.h"
#include "component/TransformComponent.h"
#include "Entity.h"

using namespace chira;

Scene::Scene()
        : handle(this->getRegistry().create()) {
    this->getRegistry().emplace<UUIDComponent>(this->handle);
    this->getRegistry().emplace<SceneTagComponent>(this->handle);

    this->getRegistry().on_construct<AngelScriptComponent>().connect<&AngelScriptComponent::onConstruct>();
    this->getRegistry().on_destroy<AngelScriptComponent>().connect<&AngelScriptComponent::onDestroy>();
}

Scene::Scene(const std::string& name)
        : Scene() {
    this->getRegistry().emplace<NameComponent>(this->handle, name);
}

Scene::~Scene() {
    this->getRegistry().clear();

    this->getRegistry().on_construct<AngelScriptComponent>().disconnect<&AngelScriptComponent::onConstruct>();
    this->getRegistry().on_destroy<AngelScriptComponent>().disconnect<&AngelScriptComponent::onDestroy>();
}

Entity* Scene::addEntity() {
    auto uuid = UUIDGenerator::getNewUUID();
    return this->addEntity(uuid);
}

Entity* Scene::addEntity(const std::string& name) {
    auto* entity = this->addEntity();
    entity->addComponent<NameComponent>(name);
    return entity;
}

Entity* Scene::addEntity(uuids::uuid uuid) {
    runtime_assert(!this->entities.contains(uuid), "Entity UUID is already present!");
    this->entities[uuid] = std::unique_ptr<Entity>{new Entity{this}};
    auto& entity = this->entities.at(uuid);
    entity->addComponent<UUIDComponent>(uuid);
    return entity.get();
}

Entity* Scene::addEntity(uuids::uuid uuid, const std::string& name) {
    auto* entity = this->addEntity(uuid);
    entity->addComponent<NameComponent>(name);
    return entity;
}

Entity* Scene::getEntity(uuids::uuid entityID) {
    if (!this->hasEntity(entityID)) {
        return nullptr;
    }
    return this->entities.at(entityID).get();
}

bool Scene::hasEntity(uuids::uuid entityID) {
    return this->entities.contains(entityID);
}

void Scene::removeEntity(uuids::uuid entityID) {
    runtime_assert(this->hasEntity(entityID), "Trying to remove an entity with a UUID that doesn't exist!");
    this->entities.erase(entityID);
}

void Scene::setupForRender(glm::vec2i size) {
    const CameraComponent* camera = nullptr;

    auto cameraView = this->getRegistry().view<CameraComponent>();
    for (auto [entity, cameraComponent] : cameraView.each()) {
        if (cameraComponent.active) {
            camera = &cameraComponent;
            break;
        }
    }
    if (!camera)
        return;

    const TransformComponent* transform = camera->transform;
    PerspectiveViewUBO::get().update(
            camera->getProjection(size),
            camera->getView(),
            transform->getPosition(),
            transform->getFrontVector());

    DirectionalLightComponent* directionalLightComponentArray[DIRECTIONAL_LIGHT_MAX] {nullptr};
    auto directionalLightsView = this->getRegistry().view<DirectionalLightComponent>();
    int directionalLightsCount = 0;
    for (auto [entity, directionalLightComponent] : directionalLightsView.each()) {
        if (directionalLightsCount < DIRECTIONAL_LIGHT_MAX) {
            directionalLightComponentArray[directionalLightsCount] = &directionalLightComponent;
        }
        directionalLightsCount++;
    }
    PointLightComponent* pointLightComponentArray[POINT_LIGHT_MAX] {nullptr};
    auto pointLightsView = this->getRegistry().view<PointLightComponent>();
    int pointLightsCount = 0;
    for (auto [entity, pointLightComponent] : pointLightsView.each()) {
        if (pointLightsCount < POINT_LIGHT_MAX) {
            pointLightComponentArray[pointLightsCount] = &pointLightComponent;
        }
        pointLightsCount++;
    }
    SpotLightComponent* spotLightComponentArray[SPOT_LIGHT_MAX] {nullptr};
    auto spotLightsView = this->getRegistry().view<SpotLightComponent>();
    int spotLightsCount = 0;
    for (auto [entity, spotLightComponent] : spotLightsView.each()) {
        if (spotLightsCount < SPOT_LIGHT_MAX) {
            spotLightComponentArray[spotLightsCount] = &spotLightComponent;
        }
        spotLightsCount++;
    }
    LightsUBO::get().update(directionalLightComponentArray, pointLightComponentArray, spotLightComponentArray,
                            {directionalLightsCount, pointLightsCount, spotLightsCount});
}

entt::registry& Scene::getRegistry() {
    return this->registry;
}

const entt::registry& Scene::getRegistry() const {
    return this->registry;
}
