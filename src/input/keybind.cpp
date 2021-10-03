#include "keybind.h"

using namespace chira;

keybind::keybind(int button, int action, const std::function<void()>& function, bool isMouse) {
    this->button = button;
    this->action = action;
    this->fire = function;
    this->mouse = isMouse;
}

int keybind::getButton() const {
    return this->button;
}

int keybind::getAction() const {
    return this->action;
}

bool keybind::isMouse() const {
    return this->mouse;
}

void keybind::run() {
    this->fire();
}
