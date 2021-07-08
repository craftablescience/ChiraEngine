#include "input.h"

keybind::keybind(int button, keybind::funcptr function) {
    this->button = button;
    this->function = function;
}

void keybind::fire() {
    (*this->function)();
}

void input::addKeybind(keybind keybind) {
    this->keybinds.push_back(keybind);
}

input::input() = default;

input::input(std::list<keybind> &keybinds) {
    this->keybinds = keybinds;
}
