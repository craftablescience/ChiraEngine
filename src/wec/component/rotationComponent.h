#pragma once

#include "abstractComponent.h"
#include <glm/glm.hpp>

class rotationComponent : public abstractComponent {
public:
    explicit rotationComponent(const glm::vec3& rot) {
        this->rotation = rot;
    }
    void render(double delta) override {}
    void stop() override {}
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
