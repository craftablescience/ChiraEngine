#pragma once

#include "abstractComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
        glm::quat rotation = glm::identity<glm::quat>();
        bool rotationChanged = true;
    };
}
