#include "light.h"

using namespace chira;

AbstractLight::AbstractLight(glm::vec3 position_, glm::vec3 ambient_, glm::vec3 diffuse_, glm::vec3 specular_):
    position(position_), ambient(ambient_), diffuse(diffuse_), specular(specular_) {}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant_, float linear_, float quadratic_):
    AbstractLight(position, ambient, diffuse, specular) {
    this->constant = constant_;
    this->linear = linear_;
    this->quadratic = quadratic_;
}

DirectionalLight::DirectionalLight(glm::vec3 direction_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular):
    AbstractLight(glm::vec3{}, ambient, diffuse, specular), direction(direction_) {}

DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 direction_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular):
    AbstractLight(position, ambient, diffuse, specular), direction(direction_) {}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutoffOuter_, float cutoffInner_, float cutoffDegree_):
    DirectionalLight(position, direction_, ambient, diffuse, specular) {
    this->cutoffOuter = cutoffOuter_;
    this->cutoffInner = cutoffInner_;
    this->cutoffDegree = cutoffDegree_;
}
