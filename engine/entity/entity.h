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
        explicit Entity(std::string name_);
        /// Initializes name to a random UUID.
        Entity();
        virtual ~Entity();
        /// Renders all this entity's children.
        virtual void render(glm::mat4 parentTransform);
        [[nodiscard]] Entity* getParent() const;
        [[nodiscard]] virtual const Root* getRoot() const;
        [[nodiscard]] std::string getName() const;
        Entity* getChild(const std::string& name_) const;
        template<typename EntityType>
        EntityType* getChild(const std::string& name_) const {
            return assert_cast<EntityType*>(this->getChild(name_));
        }
        [[nodiscard]] bool hasChild(const std::string& name_) const;
        std::string addChild(Entity* child);
        void removeChild(const std::string& name_);
        void removeAllChildren();
        [[nodiscard]] bool isVisible() const;
        void setVisible(bool visibility);
        virtual void setPosition(glm::vec3 newPos);
        virtual void setRotation(glm::quat newRot);
        virtual glm::vec3 getPosition();
        virtual glm::vec3 getGlobalPosition();
        /// Note: the global rotation is inaccessible.
        virtual glm::quat getRotation();
        virtual void translate(glm::vec3 translateByAmount);
        virtual void translateWithRotation(glm::vec3 translateByAmount);
        virtual void rotate(glm::quat rotateByAmount);
        virtual void rotate(glm::vec3 rotateByAmount);
    protected:
        Entity* parent = nullptr;
        std::string name;
        std::unordered_map<std::string, Entity*> children;
        bool visible = true;

        // The following are in local space and are relative to the parent.
        glm::vec3 position{};
        glm::quat rotation = glm::identity<glm::quat>();

        /// For internal use only.
        void setParent(Entity* newParent) {
            this->parent = newParent;
        }
    };
}
