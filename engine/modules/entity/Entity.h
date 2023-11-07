#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <fmt/core.h>

#include <core/debug/Assertions.h>
#include <core/utility/TypeString.h>
#include "component/NameComponent.h"
#include "component/TagComponents.h"
#include "component/TransformComponent.h"
#include "component/UUIDComponent.h"
#include "component/LayerComponents.h"
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
        this->addTagComponent<LayerComponent<0>>();
    }

public:
    Entity(const Entity&) = default;
    Entity& operator=(const Entity&) = default;
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;

    template<typename T, typename... Args>
    T& addComponent(Args&& ...args) {
        chiraAssert(!this->hasComponent<T>(), fmt::format("Entity already has component \"{}\"!", typeString<T>()));
        auto& component = this->scene->getRegistry().emplace<T>(this->handle, std::forward<Args>(args)...);
        if constexpr (CComponentHasTransform<T>) {
            component.transform = &this->getComponent<TransformComponent>();
        }
        return component;
    }

    template<typename T>
    void addTagComponent() {
        chiraAssert(!this->hasComponent<T>(), fmt::format("Entity already has component \"{}\"!", typeString<T>()));
        this->scene->getRegistry().emplace<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] T& getComponent() {
        chiraAssert(this->hasComponent<T>(), fmt::format("Entity doesn't have component \"{}\"!", typeString<T>()));
        return this->scene->getRegistry().get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] T& getComponent() const {
        chiraAssert(this->hasComponent<T>(), fmt::format("Entity doesn't have component \"{}\"!", typeString<T>()));
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
        chiraAssert(this->hasComponent<T>(), fmt::format("Entity doesn't have component \"{}\"!", typeString<T>()));
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
