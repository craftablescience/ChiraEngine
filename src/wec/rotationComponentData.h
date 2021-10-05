#pragma once

#include "abstractComponent.h"
#include <glm/glm.hpp>

namespace chira {
    class rotationComponentData {
    public:
        explicit rotationComponentData(const glm::vec3& rot) {
            this->rotation = rot;
        }
        void setRotation(const glm::vec3& rot) {
            this->rotation = rot;
            this->rotationChanged = true;
        }
        const glm::vec3& getRotation() {
            return this->rotation;
        }
    protected:
        glm::vec3 rotation{};
        bool rotationChanged = true;
    };
}
