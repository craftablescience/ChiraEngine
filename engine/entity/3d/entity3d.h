#pragma once

#include <nlohmann/json.hpp>
#include <glm/gtc/quaternion.hpp>
#include <entity/entity.h>

namespace chira {
    class Entity3d : public Entity {
    public:
        explicit Entity3d(Entity* parent_) : Entity(parent_) {}
        Entity3d(Entity* parent_, const std::string& name_) : Entity(parent_, name_) {}
        Entity3d() : Entity() {}
        explicit Entity3d(const std::string& name_) : Entity(name_) {}
        void render(const glm::mat4& parentTransform) override;
        virtual void setPosition(glm::vec3 newPos);
        virtual void setRotation(glm::quat newRot);
        virtual glm::vec3 getPosition();
        virtual glm::vec3 getGlobalPosition();
        /// Note: the global rotation is inaccessible.
        virtual glm::quat getRotation();
        /// The size of the entity.
        virtual glm::vec3 getAABB();
        virtual void translate(glm::vec3 translateByAmount);
        virtual void translateWithRotation(glm::vec3 translateByAmount);
        virtual void rotate(glm::quat rotateByAmount);
        virtual void rotate(glm::vec3 rotateByAmount);
    protected:
        // The following are in local space and are relative to the parent.
        glm::vec3 position{};
        glm::quat rotation = glm::identity<glm::quat>();
    };
}
