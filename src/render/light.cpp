#include "light.h"

abstractLight::abstractLight(const glm::vec3& position_, const glm::vec3& ambient_, const glm::vec3& diffuse_, const glm::vec3& specular_):
    position(position_), ambient(ambient_), diffuse(diffuse_), specular(specular_) {}

pointLight::pointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant_, float linear_, float quadratic_):
    abstractLight(position, ambient, diffuse, specular) {
    this->constant = constant_;
    this->linear = linear_;
    this->quadratic = quadratic_;
}

directionalLight::directionalLight(const glm::vec3& direction_, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular):
    abstractLight(glm::vec3{}, ambient, diffuse, specular), direction(direction_) {}

directionalLight::directionalLight(const glm::vec3& position, const glm::vec3& direction_, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular):
    abstractLight(position, ambient, diffuse, specular), direction(direction_) {}

spotLight::spotLight(const glm::vec3 &position, const glm::vec3 &direction_, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float cutoffOuter_, float cutoffInner_, float cutoffDegree_):
    directionalLight(position, direction_, ambient, diffuse, specular) {
    this->cutoffOuter = cutoffOuter_;
    this->cutoffInner = cutoffInner_;
    this->cutoffDegree = cutoffDegree_;
}
