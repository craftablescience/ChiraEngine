#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <glm/glm.hpp>

namespace chira {
    /// The base entity class. Note that the name of an entity stored in the name variable should
    /// match the name assigned to the entity in the parent's entity map.
    class Entity {
    public:
        /// Initializes name to a random UUID.
        explicit Entity(Entity* parent_);
        Entity(Entity* parent_, const std::string& name_);
        /// Initializes name to a random UUID.
        Entity();
        explicit Entity(const std::string& name_);
        virtual ~Entity();
        /// Renders all this entity's children.
        virtual void render(const glm::mat4& parentTransform);
        [[nodiscard]] Entity* getParent() const;
        std::string_view getName() const;
        Entity* getChild(const std::string& name_);
        bool hasChild(const std::string& name_);
        std::string_view addChild(Entity* child);
        void removeChild(const std::string& name_);
        void removeAllChildren();
    protected:
        Entity* parent;
        std::string name;
        std::unordered_map<std::string, Entity*> children;
        /// For internal use only.
        void setParent(Entity* newParent) {
            this->parent = newParent;
        }
    };
}
