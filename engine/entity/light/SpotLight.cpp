#include "SpotLight.h"

#include <entity/root/Frame.h>

using namespace chira;

SpotLight::SpotLight(std::string name_) : Entity(std::move(name_)) {}

SpotLight::SpotLight() : Entity() {}

void SpotLight::onAddedToTree() {
    this->getFrame()->getLightManager()->addLight(this);
}

SpotLight::~SpotLight() {
    if (auto* frame = this->getFrame())
        frame->getLightManager()->removeLight(this);
}
