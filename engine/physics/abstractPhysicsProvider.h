#pragma once

#include <glm/vec3.hpp>

namespace chira {
    class AbstractPhysicsProvider {
    public:
        virtual ~AbstractPhysicsProvider() = default;
        virtual void updatePhysics(double delta) = 0;
        virtual void stop() = 0;
        virtual void setGravity(glm::vec3 gravity) = 0;
    };
}
