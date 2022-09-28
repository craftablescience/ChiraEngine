#include "PointLight.h"

#include <entity/root/Frame.h>

using namespace chira;

PointLight::PointLight(std::string name_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 falloff)
    : Entity(std::move(name_))
    , data({{ambient, 1.f}, {diffuse, 1.f}, {specular, 1.f}, {falloff, 0.f}}) {}

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 falloff)
    : Entity()
    , data({{ambient, 1.f}, {diffuse, 1.f}, {specular, 1.f}, {falloff, 0.f}}) {}

void PointLight::onAddedToTree() {
    this->getFrame()->getLightManager()->addLight(this);
}

PointLight::~PointLight() {
    if (auto* frame = this->getFrame())
        frame->getLightManager()->removeLight(this);
}

const PointLightData* PointLight::getLightData() const {
    return &this->data;
}

glm::vec3 PointLight::getAmbient() const {
    return {this->data.ambient};
}

void PointLight::setAmbient(glm::vec3 newAmbient) {
    this->data.ambient = {newAmbient, 1.f};
}

glm::vec3 PointLight::getDiffuse() const {
    return {this->data.diffuse};
}

void PointLight::setDiffuse(glm::vec3 newDiffuse) {
    this->data.diffuse = {newDiffuse, 1.f};
}

glm::vec3 PointLight::getSpecular() const {
    return {this->data.specular};
}

void PointLight::setSpecular(glm::vec3 newSpecular) {
    this->data.specular = {newSpecular, 1.f};
}

float PointLight::getConstant() const {
    return this->data.falloff.x;
}

void PointLight::setConstant(float newConstant) {
    this->data.falloff.x = newConstant;
}

float PointLight::getLinear() const {
    return this->data.falloff.y;
}

void PointLight::setLinear(float newLinear) {
    this->data.falloff.y = newLinear;
}

float PointLight::getQuadratic() const {
    return this->data.falloff.z;
}

void PointLight::setQuadratic(float newQuadratic) {
    this->data.falloff.z = newQuadratic;
}
