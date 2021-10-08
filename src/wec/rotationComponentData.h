#pragma once

#include "abstractComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// Why does this not have a default constructor??
#define QUAT_IDENTITY glm::quat{1,0,0,0}

namespace chira {
    class rotationComponentData {
    public:
        explicit rotationComponentData(const glm::quat& rot) {
            this->rotation = rot;
        }
        void setRotation(const glm::quat& rot) {
            this->rotation = rot;
            this->rotationChanged = true;
        }
        const glm::quat& getRotation() {
            return this->rotation;
        }
    protected:
        glm::quat rotation = QUAT_IDENTITY;
        bool rotationChanged = true;
    };
}
