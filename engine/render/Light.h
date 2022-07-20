#pragma once

#include <glm/glm.hpp>

namespace chira {

struct AbstractLight {
    AbstractLight(glm::vec3 position_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_);
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight : public AbstractLight {
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant_, float linear_, float quadratic_);
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight : public AbstractLight {
    DirectionalLight(glm::vec3 direction_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    DirectionalLight(glm::vec3 position, glm::vec3 direction_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    glm::vec3 direction;
};

struct SpotLight : public DirectionalLight {
    SpotLight(glm::vec3 position, glm::vec3 direction_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutoffOuter_, float cutoffInner_, float cutoffDegree_);
    float cutoffOuter;
    float cutoffInner;
    float cutoffDegree;
};

} // namespace chira
