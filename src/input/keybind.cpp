#include "keybind.h"

keybind::keybind(int button, int action, const std::function<void(engine*)>& function, bool sticky) {
    this->button = button;
    this->action = action;
    this->fire = function;
    this->sticky = sticky;
    this->justFired = false;
}

int keybind::getButton() const {
    return this->button;
}

int keybind::getAction() const {
    return this->action;
}

bool keybind::isSticky() const {
    return this->sticky;
}

bool keybind::wasJustFired() const {
    return this->justFired;
}

void keybind::run(engine* engine) {
    if (this->sticky || !this->justFired) {
        this->fire(engine);
        this->justFired = true;
    }
}

void keybind::unstick() {
    this->justFired = false;
}
