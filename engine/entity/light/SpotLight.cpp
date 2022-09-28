#include "SpotLight.h"

#include <entity/root/Frame.h>

using namespace chira;

SpotLight::SpotLight(std::string name_, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 falloff, glm::vec2 cutoff)
    : Entity(std::move(name_))
    , data({{diffuse, 1.f}, {specular, 1.f}, {falloff, 0.f}, {cutoff, 0.f, 0.f}}) {}

SpotLight::SpotLight(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 falloff, glm::vec2 cutoff)
    : Entity()
    , data({{diffuse, 1.f}, {specular, 1.f}, {falloff, 0.f}, {cutoff, 0.f, 0.f}}) {}

void SpotLight::onAddedToTree() {
    this->getFrame()->getLightManager()->addLight(this);
}

SpotLight::~SpotLight() {
    if (auto* frame = this->getFrame())
        frame->getLightManager()->removeLight(this);
}

const SpotLightData* SpotLight::getLightData() const {
    return &this->data;
}

glm::vec3 SpotLight::getDiffuse() const {
    return {this->data.diffuse};
}

void SpotLight::setDiffuse(glm::vec3 newDiffuse) {
    this->data.diffuse = {newDiffuse, 1.f};
}

glm::vec3 SpotLight::getSpecular() const {
    return {this->data.specular};
}

void SpotLight::setSpecular(glm::vec3 newSpecular) {
    this->data.specular = {newSpecular, 1.f};
}

float SpotLight::getConstant() const {
    return this->data.falloff.x;
}

void SpotLight::setConstant(float newConstant) {
    this->data.falloff.x = newConstant;
}

float SpotLight::getLinear() const {
    return this->data.falloff.y;
}

void SpotLight::setLinear(float newLinear) {
    this->data.falloff.y = newLinear;
}

float SpotLight::getQuadratic() const {
    return this->data.falloff.z;
}

void SpotLight::setQuadratic(float newQuadratic) {
    this->data.falloff.z = newQuadratic;
}

float SpotLight::getInnerCone() const {
    return this->data.cutoff.x;
}

void SpotLight::setInnerCone(float newInnerCone) {
    this->data.cutoff.x = newInnerCone;
}

float SpotLight::getOuterCone() const {
    return this->data.cutoff.y;
}

void SpotLight::setOuterCone(float newOuterCone) {
    this->data.cutoff.y = newOuterCone;
}
