#pragma once

#include <glm/glm.hpp>

struct abstractLight {
    abstractLight(const glm::vec3& position_, const glm::vec3& ambient_, const glm::vec3& diffuse_, const glm::vec3& specular_);
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct pointLight : public abstractLight {
    pointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant_, float linear_, float quadratic_);
    float constant;
    float linear;
    float quadratic;
};

struct directionalLight : public abstractLight {
    directionalLight(const glm::vec3& direction_, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
    directionalLight(const glm::vec3& position, const glm::vec3& direction_, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);
    glm::vec3 direction;
};

struct spotLight : public directionalLight {
    spotLight(const glm::vec3& position, const glm::vec3& direction_, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float cutoffOuter_, float cutoffInner_, float cutoffDegree_);
    float cutoffOuter;
    float cutoffInner;
    float cutoffDegree;
};
