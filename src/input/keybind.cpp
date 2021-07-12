#include "keybind.h"

keybind::keybind(int button, int action, const std::function<void(engine*)>& function) {
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

void keybind::run(engine* engine) {
    this->fire(engine);
}
