#include "PointLight.h"

#include <entity/root/Frame.h>

using namespace chira;

PointLight::PointLight(std::string name_) : Entity(std::move(name_)) {}

PointLight::PointLight() : Entity() {}

void PointLight::onAddedToTree() {
    this->getFrame()->getLightManager()->addLight(this);
}

PointLight::~PointLight() {
    if (auto* frame = this->getFrame())
        frame->getLightManager()->removeLight(this);
}
