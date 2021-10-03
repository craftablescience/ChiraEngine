#pragma once

#include <glm/vec3.hpp>

namespace chira {
    class abstractPhysicsProvider {
    public:
        virtual void updatePhysics(double delta) = 0;
        virtual void stop() = 0;
        virtual void setGravity(const glm::vec3& gravity) = 0;
    };
}
