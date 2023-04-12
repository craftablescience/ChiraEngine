#pragma once

#include "TransformComponent.h"

namespace chira {

// These are arbitrary amounts, be careful not to increase by too much or the GPU will hate you
constexpr int DIRECTIONAL_LIGHT_MAX = 4;
constexpr int POINT_LIGHT_MAX = 32;
constexpr int SPOT_LIGHT_MAX = 16;

struct DirectionalLightComponent {
    TransformComponent* transform;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLightComponent {
    TransformComponent* transform;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    // x: constant, y: linear, z: quadratic
    glm::vec3 falloff;
};

struct SpotLightComponent {
    TransformComponent* transform;
    glm::vec3 diffuse;
    glm::vec3 specular;
    // x: constant, y: linear, z: quadratic
    glm::vec3 falloff;
    // x: inner cutoff angle, y: outer cutoff angle
    glm::vec2 cutoff;
};

} // namespace chira
