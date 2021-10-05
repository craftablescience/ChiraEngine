#pragma once

#include "abstractComponent.h"
#include <glm/glm.hpp>

namespace chira {
    class positionComponentData {
    public:
        explicit positionComponentData(const glm::vec3& pos) {
            this->position = pos;
        }
        void setPosition(const glm::vec3& pos) {
            this->position = pos;
            this->positionChanged = true;
        }
        const glm::vec3& getPosition() {
            return this->position;
        }
    protected:
        glm::vec3 position{};
        bool positionChanged = true;
    };
}
