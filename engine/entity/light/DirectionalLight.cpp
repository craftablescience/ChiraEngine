#include "DirectionalLight.h"

#include <entity/root/Frame.h>

using namespace chira;

DirectionalLight::DirectionalLight(std::string name_) : Entity(std::move(name_)) {}

DirectionalLight::DirectionalLight() : Entity() {}

void DirectionalLight::onAddedToTree() {
    this->getFrame()->getLightManager()->addLight(this);
}

DirectionalLight::~DirectionalLight() {
    if (auto* frame = this->getFrame())
        frame->getLightManager()->removeLight(this);
}
