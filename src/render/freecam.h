#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../utility/axis.h"
#include "../core/engine.h"
#include "perspectiveCamera.h"

class freecam : public perspectiveCamera {
public:
    explicit freecam(engine* engine, float newYaw = -90.0f, float newPitch = 0.0f, glm::vec3 newPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 currentUp = glm::vec3(0.0f, 1.0f, 0.0f), float newZoom = 45.0f, float newSpeed = 2.5f, float newSensitivity = 0.1f, bool controls = true);
    void init(engine* engine) override;
    void translate(glm::vec3 offset, double delta);
    void translateLocal(signedAxis axis, double delta);
    void setActive(bool newActive) override;
protected:
    float movementSpeed;
    float mouseSensitivity;
    bool capturedMouse = true;
    void setupKeybinds(engine* engine) const;
};
