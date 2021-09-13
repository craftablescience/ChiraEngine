#pragma once

#include "positionComponent.h"
#include "rotationComponent.h"
#include "../../render/mesh.h"

class meshComponent : public positionComponent, public rotationComponent {
public:
    explicit meshComponent(mesh* mesh_, const glm::vec3& pos, const glm::vec3& rot);
    void render(double delta) override;
    void stop() override;
private:
    mesh* meshPtr = nullptr;
};
