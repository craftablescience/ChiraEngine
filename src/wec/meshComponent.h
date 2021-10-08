#pragma once

#include "positionComponentData.h"
#include "rotationComponentData.h"
#include "../render/mesh.h"
#include <glm/gtc/quaternion.hpp>

namespace chira {
    class meshComponent : public abstractComponent, public positionComponentData, public rotationComponentData {
    public:
        explicit meshComponent(mesh* mesh_, const glm::vec3& pos = glm::vec3{}, const glm::quat& rot = QUAT_IDENTITY);
        [[nodiscard]] mesh* getMesh() const {
            return this->meshPtr;
        }
        void render(double delta) override;
        ~meshComponent() override;
    private:
        mesh* meshPtr = nullptr;
    };
}
