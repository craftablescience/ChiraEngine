#pragma once

#include "positionComponentData.h"
#include "rotationComponentData.h"
#include "../resource/meshResource.h"
#include <glm/gtc/quaternion.hpp>

namespace chira {
    class meshComponent : public abstractComponent, public positionComponentData, public rotationComponentData {
    public:
        explicit meshComponent(meshResource* mesh_, const glm::vec3& pos = glm::vec3{}, const glm::quat& rot = glm::identity<glm::quat>());
        [[nodiscard]] meshResource* getMesh() const {
            return this->meshPtr;
        }
        void render(double delta) override;
        ~meshComponent() override;
    private:
        meshResource* meshPtr = nullptr;
    };
}
