#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <utility/debug/assertions.h>
#include <utility/math/matrix.h>

namespace chira {
    class Root;

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
        virtual void render(glm::mat4 parentTransform);
        [[nodiscard]] Entity* getParent() const;
        [[nodiscard]] virtual const Root* getRoot() const;
        [[nodiscard]] std::string getName() const;
        Entity* getChild(const std::string& name_) const {
            return this->children.at(name_);
        }
        template<typename EntityType>
        EntityType* getChild(const std::string& name_) const {
            return assert_cast<EntityType*>(this->getChild(name_));
        }
        [[nodiscard]] bool hasChild(const std::string& name_) const;
        std::string addChild(Entity* child);
        void removeChild(const std::string& name_);
        void removeAllChildren();
        virtual void setPosition(glm::vec3 newPos);
        virtual void setRotation(glm::quat newRot);
        virtual glm::vec3 getPosition();
        virtual glm::vec3 getGlobalPosition();
        /// Note: the global rotation is inaccessible.
        virtual glm::quat getRotation();
        /// The size of the entity.
        virtual glm::vec3 getAABB() const;
        virtual void translate(glm::vec3 translateByAmount);
        virtual void translateWithRotation(glm::vec3 translateByAmount);
        virtual void rotate(glm::quat rotateByAmount);
        virtual void rotate(glm::vec3 rotateByAmount);
    protected:
        Entity* parent;
        std::string name;
        std::unordered_map<std::string, Entity*> children;

        // The following are in local space and are relative to the parent.
        glm::vec3 position{};
        glm::quat rotation = glm::identity<glm::quat>();

        /// For internal use only.
        void setParent(Entity* newParent) {
            this->parent = newParent;
        }
    };
}
