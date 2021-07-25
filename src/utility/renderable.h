#pragma once

#include "compilable.h"
#include "../render/shader.h"
#include <glm/glm.hpp>

class renderable : public compilable {
public:
    renderable() : model(1.0f) {}
    virtual void render(shader* shader) = 0;
    glm::mat4* getModel() {
        return &(this->model);
    };
protected:
    glm::mat4 model;
};
