#pragma once

#include "abstractComponent.h"
#include <glm/glm.hpp>

class positionComponent : public abstractComponent {
public:
    explicit positionComponent(const glm::vec3& pos) {
        this->position = pos;
    }
    void render(double delta) override {}
    void stop() override {}
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
