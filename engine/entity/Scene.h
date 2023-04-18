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
    friend class Layer;

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

    [[nodiscard]] bool hasEntity(uuids::uuid entityID);

    void removeEntity(uuids::uuid entityID);

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
