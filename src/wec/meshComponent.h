#pragma once

#include "positionComponentData.h"
#include "rotationComponentData.h"
#include "../render/mesh.h"

namespace chira {
    class meshComponent : public abstractComponent, public positionComponentData, public rotationComponentData {
    public:
        meshComponent(mesh* mesh_, const glm::vec3& pos, const glm::vec3& rot);
        void render(double delta) override;
        ~meshComponent() override;
    private:
        mesh* meshPtr = nullptr;
    };
}
