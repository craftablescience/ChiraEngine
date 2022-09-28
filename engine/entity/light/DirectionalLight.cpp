#include "DirectionalLight.h"

#include <entity/root/Frame.h>

using namespace chira;

DirectionalLight::DirectionalLight(std::string name_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Entity(std::move(name_))
    , data({{ambient, 1.f}, {diffuse, 1.f}, {specular, 1.f}}) {}

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Entity()
    , data({{ambient, 1.f}, {diffuse, 1.f}, {specular, 1.f}}) {}

void DirectionalLight::onAddedToTree() {
    this->getFrame()->getLightManager()->addLight(this);
}

DirectionalLight::~DirectionalLight() {
    if (auto* frame = this->getFrame())
        frame->getLightManager()->removeLight(this);
}

const DirectionalLightData* DirectionalLight::getLightData() const {
    return &this->data;
}

glm::vec3 DirectionalLight::getAmbient() const {
    return {this->data.ambient};
}

void DirectionalLight::setAmbient(glm::vec3 newAmbient) {
    this->data.ambient = {newAmbient, 1.f};
}

glm::vec3 DirectionalLight::getDiffuse() const {
    return {this->data.diffuse};
}

void DirectionalLight::setDiffuse(glm::vec3 newDiffuse) {
    this->data.diffuse = {newDiffuse, 1.f};
}

glm::vec3 DirectionalLight::getSpecular() const {
    return {this->data.specular};
}

void DirectionalLight::setSpecular(glm::vec3 newSpecular) {
    this->data.specular = {newSpecular, 1.f};
}
