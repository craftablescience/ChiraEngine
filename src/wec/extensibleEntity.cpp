#include "extensibleEntity.h"

void extensibleEntity::setInitFunction(const std::function<void()>& function) {
    this->initFunction = function;
}

void extensibleEntity::setPreRenderFunction(const std::function<void(double)>& function) {
    this->preRenderFunction = function;
}

void extensibleEntity::setPostRenderFunction(const std::function<void(double)>& function) {
    this->postRenderFunction = function;
}

void extensibleEntity::setStopFunction(const std::function<void()>& function) {
    this->stopFunction = function;
}

void extensibleEntity::init() {
    this->initFunction();
}

void extensibleEntity::preRender(double delta) {
    this->preRenderFunction(delta);
}

void extensibleEntity::postRender(double delta) {
    this->postRenderFunction(delta);
}

void extensibleEntity::stop() {
    this->stopFunction();
}
