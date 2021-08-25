#pragma once

#include "positionComponent.h"
#include "../../render/mesh.h"

class meshComponent : public positionComponent {
public:
    explicit meshComponent(mesh* mesh_, const glm::vec3& pos);
    void render(double delta) override;
    void stop() override;
private:
    mesh* meshPtr = nullptr;
};
