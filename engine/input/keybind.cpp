#include "keybind.h"

using namespace chira;

Keybind::Keybind(int button, int action, const std::function<void()>& function, bool isMouse) {
    this->button = button;
    this->action = action;
    this->fire = function;
    this->mouse = isMouse;
}

int Keybind::getButton() const {
    return this->button;
}

int Keybind::getAction() const {
    return this->action;
}

bool Keybind::isMouse() const {
    return this->mouse;
}

void Keybind::run() {
    this->fire();
}
