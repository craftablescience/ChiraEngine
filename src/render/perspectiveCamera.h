#ifndef BASICGAMEENGINE_PERSPECTIVECAMERA_H
#define BASICGAMEENGINE_PERSPECTIVECAMERA_H


#include <glm/glm.hpp>
#include "../utility/axis.h"
#include "../core/engine.h"
#include "abstractCamera.h"

// Largely taken from https://learnopengl.com/Getting-started/Camera
class perspectiveCamera : public abstractCamera {
public:
    explicit perspectiveCamera(engine* engine, float newYaw, float newPitch, glm::vec3 newPosition, glm::vec3 currentUp = glm::vec3(0.0f, 1.0f, 0.0f), float newZoom = 45.0f);
    void translate(glm::vec3 offset, float amount, double delta) override;
    void translateLocal(signedAxis axis, float amount, double delta);
    void rotate(float rotation, axis rotationAxis, double delta) override;
    void setPosition(glm::vec3 newPosition) override;
    void setRotation(glm::vec3 rotation) override;
    void setScreenDimensions(int width, int height) override;
    glm::mat4* getViewMatrix() override;
protected:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float zoom;
    void updateCameraVectors();
};


#endif //BASICGAMEENGINE_PERSPECTIVECAMERA_H