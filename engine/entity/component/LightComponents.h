#pragma once

#include "TransformComponent.h"

namespace chira {

// These are arbitrary amounts, be careful not to increase by too much or the GPU will hate you
constexpr int DIRECTIONAL_LIGHT_MAX = 4;
constexpr int POINT_LIGHT_MAX = 32;
constexpr int SPOT_LIGHT_MAX = 16;

struct DirectionalLightComponent {
    TransformComponent* transform;
    glm::vec3 ambient{0.1f};
    glm::vec3 diffuse{0.8f};
    glm::vec3 specular{0.1f};
};

struct PointLightComponent {
    TransformComponent* transform;
    glm::vec3 ambient{0.1f};
    glm::vec3 diffuse{0.8f};
    glm::vec3 specular{0.1f};
    // x: constant, y: linear, z: quadratic
    // default is 100 units
    glm::vec3 falloff{1.f, 0.045f, 0.0075f};
};

struct SpotLightComponent {
    TransformComponent* transform;
    glm::vec3 diffuse{0.8f};
    glm::vec3 specular{0.1f};
    // x: constant, y: linear, z: quadratic
    // default is 100 units
    glm::vec3 falloff{1.f, 0.045f, 0.0075f};
    // x: inner cutoff angle, y: outer cutoff angle
    glm::vec2 cutoff{glm::radians(45.f), glm::radians(60.f)};
};

} // namespace chira
