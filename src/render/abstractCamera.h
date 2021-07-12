#ifndef BASICGAMEENGINE_ABSTRACTCAMERA_H
#define BASICGAMEENGINE_ABSTRACTCAMERA_H


#include "../utility/axis.h"

class abstractCamera {
public:
    virtual void translate(glm::vec3 offset, double delta) = 0;
    virtual void rotate(float rotation, axis rotationAxis, double delta) = 0;
    virtual void setPosition(glm::vec3 position) = 0;
    virtual void setRotation(glm::vec3 rotation) = 0;
    virtual void setScreenDimensions(int width, int height) = 0;
    virtual glm::mat4* getProjectionMatrix() {
        return &(this->projection);
    }
    virtual glm::mat4* getViewMatrix() {
        return &(this->view);
    }
    [[nodiscard]] bool isCurrent() const {
        return this->current;
    }
    void setCurrent(bool newCurrent) {
        this->current = newCurrent;
    }
protected:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 position;
    bool current;
};


#endif //BASICGAMEENGINE_ABSTRACTCAMERA_H
