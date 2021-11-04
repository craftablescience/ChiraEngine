#pragma once

#include "../entity.h"

namespace chira {
    class entity2d : public entity {
    public:
        explicit entity2d(entity* parent_) : entity(parent_) {}
        entity2d(entity* parent_, const std::string& name_) : entity(parent_, name_) {}
        entity2d() : entity() {}
        explicit entity2d(const std::string& name_) : entity() {}
        void render(const glm::mat4& parentTransform) override;
        void setPosition(const glm::vec2& newPos);
        void setRotation(const glm::quat& newRot);
        const glm::vec2& getPosition();
        const glm::quat& getRotation();
        void translate(const glm::vec2& translateByAmount);
        void rotate(const glm::quat& rotateByAmount);
    protected:
        // The following are in local space and are relative to the parent.
        glm::vec2 position{};
        glm::quat rotation{};
    };
}
