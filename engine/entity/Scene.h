#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <entt/entt.hpp>
#include <math/Types.h>
#include "component/CameraComponent.h"
#include "component/NameComponent.h"
#include "component/UUIDComponent.h"

namespace chira {

class Entity;

class Scene {
    friend class Viewport;

protected:
    Scene();

    explicit Scene(const std::string& name);

public:
    ~Scene();

    Entity* addEntity();

    Entity* addEntity(const std::string& name);

    Entity* addEntity(uuids::uuid uuid);

    Entity* addEntity(uuids::uuid uuid, const std::string& name);

    [[nodiscard]] Entity* getEntity(uuids::uuid entityID);

    template<typename... T>
    auto getEntities() const {
        return this->getRegistry().view<T...>();
    }

    template<typename... T>
    auto getEntities(auto&& exclude) const {
        return this->getRegistry().view<T...>(exclude);
    }

    [[nodiscard]] const auto& getEntities() const {
        return this->entities;
    }

    [[nodiscard]] bool hasEntity(uuids::uuid entityID);

    void removeEntity(uuids::uuid entityID);

    template<typename T, typename... Args>
    T& addComponent(Args&& ...args) {
        static_assert(!CComponentHasTransform<T>, "Adding components to a scene that require a transform will break horribly!");
        runtime_assert(!this->hasComponent<T>(), "Scene already has this component!");
        return this->getRegistry().emplace<T>(this->handle, std::forward<Args>(args)...);
    }

    template<typename T>
    void addTagComponent() {
        runtime_assert(!this->hasComponent<T>(), "Scene already has this component!");
        this->getRegistry().emplace<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] T& getComponent() {
        runtime_assert(this->hasComponent<T>(), "Scene doesn't have this component!");
        return this->getRegistry().get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] const T& getComponent() const {
        runtime_assert(this->hasComponent<T>(), "Scene doesn't have this component!");
        return this->getRegistry().get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] T* tryGetComponent() {
        return this->getRegistry().try_get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] const T* tryGetComponent() const {
        return this->getRegistry().try_get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] bool hasComponent() const {
        return this->getRegistry().all_of<T>(this->handle);
    }

    template<typename T>
    void tryRemoveComponent() {
        this->getRegistry().remove<T>(this->handle);
    }

    template<typename T>
    void removeComponent() {
        runtime_assert(this->hasComponent<T>(), "Scene doesn't have this component!");
        this->getRegistry().erase<T>(this->handle);
    }

    CameraComponent* getCamera();

    void setupForRender(glm::vec2i size);

    [[nodiscard]] entt::registry& getRegistry();

    [[nodiscard]] const entt::registry& getRegistry() const;

    [[nodiscard]] std::string getName() const {
        if (auto nameComponent = this->getRegistry().try_get<NameComponent>(this->handle)) {
            return nameComponent->name;
        }
        return uuids::to_string(this->getRegistry().get<UUIDComponent>(this->handle).uuid);
    }

    [[nodiscard]] uuids::uuid getUUID() const {
        return this->getRegistry().get<UUIDComponent>(this->handle).uuid;
    }

    [[nodiscard]] entt::entity getRawHandle() const {
        return this->handle;
    }

    bool operator==(const Scene& other) const {
        return this->handle == other.handle;
    }

    bool operator!=(const Scene& other) const {
        return !(*this == other);
    }

private:
    std::unordered_map<uuids::uuid, std::unique_ptr<Entity>> entities;
    entt::registry registry;
    entt::entity handle;
};

} // namespace chira
