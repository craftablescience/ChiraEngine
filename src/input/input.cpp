#include "input.h"

keybind::keybind(int button, int action, const std::function<void ()>& function) {
    this->button = button;
    this->action = action;
    this->fire = function;
}

int keybind::getButton() const {
    return this->button;
}

int keybind::getAction() const {
    return this->action;
}

void input::addKeybind(const keybind& keybind) {
    this->keybinds.push_back(keybind);
}
