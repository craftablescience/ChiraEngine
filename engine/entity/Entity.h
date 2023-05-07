#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include <core/Assertions.h>
#include "component/NameComponent.h"
#include "component/TagComponents.h"
#include "component/TransformComponent.h"
#include "component/UUIDComponent.h"
#include "Scene.h"

namespace chira {

/// The base entity class.
class Entity {
    friend Scene;

protected:
    explicit Entity(Scene* scene_)
            : scene(scene_)
            , handle(scene_->getRegistry().create()) {
        this->addComponent<TransformComponent>();
    }

public:
    Entity(const Entity&) = default;
    Entity& operator=(const Entity&) = default;
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;

    template<typename T, typename... Args>
    T& addComponent(Args&& ...args) {
        runtime_assert(!this->hasComponent<T>(), "Entity already has this component!");
        auto& component = this->scene->getRegistry().emplace<T>(this->handle, std::forward<Args>(args)...);
        if constexpr (CComponentHasTransform<T>) {
            component.transform = &this->getComponent<TransformComponent>();
        }
        return component;
    }

    template<typename T>
    void addTagComponent() {
        runtime_assert(!this->hasComponent<T>(), "Entity already has this component!");
        this->scene->getRegistry().emplace<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] T& getComponent() {
        runtime_assert(this->hasComponent<T>(), "Entity doesn't have this component!");
        return this->scene->getRegistry().get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] T& getComponent() const {
        runtime_assert(this->hasComponent<T>(), "Entity doesn't have this component!");
        return this->scene->getRegistry().get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] T* tryGetComponent() const {
        return this->scene->getRegistry().try_get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] bool hasComponent() const {
        return this->scene->getRegistry().all_of<T>(this->handle);
    }

    template<typename T>
    void tryRemoveComponent() {
        this->scene->getRegistry().remove<T>(this->handle);
    }

    template<typename T>
    void removeComponent() {
        runtime_assert(this->hasComponent<T>(), "Entity doesn't have this component!");
        this->scene->getRegistry().erase<T>(this->handle);
    }

    [[nodiscard]] bool getVisible() const {
        return !this->hasComponent<NoRenderTagComponent>();
    }

    void setVisible(bool visible) {
        if (visible) {
            this->tryRemoveComponent<NoRenderTagComponent>();
        } else if (!this->hasComponent<NoRenderTagComponent>()) {
            this->addTagComponent<NoRenderTagComponent>();
        }
    }

    [[nodiscard]] std::string getName() const {
        if (auto nameComponent = this->tryGetComponent<NameComponent>()) {
            return nameComponent->name;
        }
        return uuids::to_string(this->getComponent<UUIDComponent>().uuid);
    }

    [[nodiscard]] TransformComponent& getTransform() {
        return this->scene->getRegistry().get<TransformComponent>(this->handle);
    }

    [[nodiscard]] const TransformComponent& getTransform() const {
        return this->scene->getRegistry().get<TransformComponent>(this->handle);
    }

    uuids::uuid getUUID() {
        return this->getComponent<UUIDComponent>().uuid;
    }

    uuids::uuid getSceneUUID() {
        return this->scene->getUUID();
    }

    [[nodiscard]] entt::entity getRawHandle() const {
        return this->handle;
    }

    explicit operator bool() const {
        return this->scene->getRegistry().valid(this->handle);
    }

    bool operator!() const {
        return !static_cast<bool>(*this);
    }

    bool operator==(const Entity& other) const {
        return this->handle == other.handle && this->scene == other.scene;
    }

    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

protected:
    Scene* scene = nullptr;
    entt::entity handle = entt::null;
};

} // namespace chira
