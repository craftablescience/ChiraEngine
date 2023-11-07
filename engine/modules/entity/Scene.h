#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <entt/entt.hpp>
#include <fmt/core.h>

#include <core/debug/Assertions.h>
#include <core/math/Types.h>
#include <core/utility/TypeString.h>
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
        chiraAssert(!this->hasComponent<T>(), fmt::format("Scene already has component \"{}\"!", typeString<T>()));
        return this->getRegistry().emplace<T>(this->handle, std::forward<Args>(args)...);
    }

    template<typename T>
    void addTagComponent() {
        chiraAssert(!this->hasComponent<T>(), fmt::format("Scene already has component \"{}\"!", typeString<T>()));
        this->getRegistry().emplace<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] T& getComponent() {
        chiraAssert(this->hasComponent<T>(), fmt::format("Scene doesn't have component \"{}\"!", typeString<T>()));
        return this->getRegistry().get<T>(this->handle);
    }

    template<typename T>
    [[nodiscard]] const T& getComponent() const {
        chiraAssert(this->hasComponent<T>(), fmt::format("Scene doesn't have component \"{}\"!", typeString<T>()));
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
        chiraAssert(this->hasComponent<T>(), fmt::format("Scene doesn't have component \"{}\"!", typeString<T>()));
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
