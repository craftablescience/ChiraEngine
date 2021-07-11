#ifndef BASICGAMEENGINE_GLRENDERABLE_H
#define BASICGAMEENGINE_GLRENDERABLE_H


#include "glCompilable.h"
#include "../render/shader.h"
#include <glm/glm.hpp>

class glRenderable : public glCompilable {
public:
    glRenderable() : model(1.0f) {}
    virtual void render(shader* shader) = 0;
    glm::mat4* getModel() {
        return &(this->model);
    };
protected:
    glm::mat4 model;
};


#endif //BASICGAMEENGINE_GLRENDERABLE_H
