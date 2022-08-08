#include "Light.h"

using namespace chira;

[[maybe_unused]]
static ConVar max_point_lights{"max_point_lights", 64, CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)
[[maybe_unused]]
static ConVar max_directional_lights{"max_directional_lights", 1, CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)
[[maybe_unused]]
static ConVar max_spot_lights{"max_spot_lights", 8, CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

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
