#include "extensibleEntity.h"

void extensibleEntity::setInitFunction(const std::function<void()>& function) {
    this->initFunction = function;
}

void extensibleEntity::setRenderFunction(const std::function<void(double)>& function) {
    this->renderFunction = function;
}

void extensibleEntity::setStopFunction(const std::function<void()>& function) {
    this->stopFunction = function;
}

void extensibleEntity::init() {
    this->initFunction();
}

void extensibleEntity::render(double delta) {
    this->renderFunction(delta);
}

void extensibleEntity::stop() {
    this->stopFunction();
}
