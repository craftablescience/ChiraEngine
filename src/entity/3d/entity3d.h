#pragma once

#include <nlohmann/json.hpp>
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
        virtual const glm::vec3& getPosition();
        /// Note: to reduce complexity, currently only the global position is accessible.
        virtual glm::vec3 getGlobalPosition();
        virtual const glm::quat& getRotation();
        virtual void translate(const glm::vec3& translateByAmount);
        virtual void rotate(const glm::quat& rotateByAmount);
    protected:
        // The following are in local space and are relative to the parent.
        glm::vec3 position{};
        glm::quat rotation{};
    };
}
