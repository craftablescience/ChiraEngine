#pragma once

#include <unordered_map>
#include <string>
#include <string_view>
#include <glm/glm.hpp>

namespace chira {
    /// The base entity class. Note that the name of an entity stored in the name variable should
    /// match the name assigned to the entity in the parent's entity map.
    class entity {
    public:
        /// Initializes name to a random UUID.
        explicit entity(entity* parent_);
        entity(entity* parent_, const std::string& name_);
        /// Initializes name to a random UUID.
        entity();
        explicit entity(const std::string& name_);
        virtual ~entity();
        /// Renders all this entity's children.
        virtual void render(const glm::mat4& parentTransform);
        [[nodiscard]] entity* getParent() const;
        std::string_view getName() const;
        entity* getChild(const std::string& name_);
        bool hasChild(const std::string& name_);
        std::string_view addChild(entity* child);
        void removeChild(const std::string& name_);
        void removeAllChildren();
    protected:
        entity* parent;
        std::string name;
        std::unordered_map<std::string,entity*> children{};
        /// For internal use only.
        void setParent(entity* newParent) {
            this->parent = newParent;
        }
    };
}
