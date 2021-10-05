#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "../utility/uuidGenerator.h"
#include "abstractComponent.h"
#include "abstractEntity.h"
#include "abstractWorld.h"

// todo: make a factory so you can create a component from a string name

namespace chira {
    class componentManager {
    public:
        static uuids::uuid addComponent(abstractComponent* component);
        static uuids::uuid addEntity(abstractEntity* entity);
        static uuids::uuid addWorld(abstractWorld* world);
        template<typename C> static abstractComponent* getComponent(const uuids::uuid& uuid) {
            return dynamic_cast<C*>(componentManager::components[uuid]);
        }
        template<typename C> static abstractEntity* getEntity(const uuids::uuid& uuid) {
            return dynamic_cast<C*>(componentManager::entities[uuid]);
        }
        template<typename C> static abstractWorld* getWorld(const uuids::uuid& uuid) {
            return dynamic_cast<C*>(componentManager::worlds[uuid]);
        }
        static void removeComponent(const uuids::uuid& uuid);
        static void removeEntity(const uuids::uuid& uuid);
        static void removeWorld(const uuids::uuid& uuid);
        static void render(double delta);
        static void clear();
    private:
        static inline std::unordered_map<uuids::uuid,abstractComponent*> components{};
        static inline std::unordered_map<uuids::uuid,abstractEntity*> entities{};
        static inline std::unordered_map<uuids::uuid,abstractWorld*> worlds{};
    };
}
