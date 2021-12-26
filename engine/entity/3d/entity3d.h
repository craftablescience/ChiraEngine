#pragma once

#include <nlohmann/json.hpp>
#include <glm/gtc/quaternion.hpp>
#include <entity/entity.h>

namespace chira {
    class entity3d : public entity {
    public:
        explicit entity3d(entity* parent_) : entity(parent_) {}
        entity3d(entity* parent_, const std::string& name_) : entity(parent_, name_) {}
        entity3d() : entity() {}
        explicit entity3d(const std::string& name_) : entity(name_) {}
        void render(const glm::mat4& parentTransform) override;
        virtual void setPosition(const glm::vec3& newPos);
        virtual void setRotation(const glm::quat& newRot);
        virtual glm::vec3 getPosition();
        virtual glm::vec3 getGlobalPosition();
        /// Note: the global rotation is inaccessible.
        virtual glm::quat getRotation();
        /// The size of the entity.
        virtual glm::vec3 getAABB();
        virtual void translate(const glm::vec3& translateByAmount);
        virtual void translateWithRotation(const glm::vec3& translateByAmount);
        virtual void rotate(const glm::quat& rotateByAmount);
        virtual void rotate(const glm::vec3& rotateByAmount);
    protected:
        // The following are in local space and are relative to the parent.
        glm::vec3 position{};
        glm::quat rotation = glm::identity<glm::quat>();
    };
}
