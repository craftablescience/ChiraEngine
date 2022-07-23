#include "InputManager.h"

#include <utility>
#include <config/ConEntry.h>

using namespace chira;

[[maybe_unused]]
static ConVar input_invert_x_axis{"input_invert_x_axis", false, "Invert the X axis for a mouse or controller.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)
[[maybe_unused]]
static ConVar input_invert_y_axis{"input_invert_y_axis", false, "Invert the Y axis for a mouse or controller.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

InputKeyButton::InputKeyButton(Key key_, InputKeyEventType eventType_, std::function<void()> func_)
    : key(key_)
    , eventType(eventType_)
    , func(std::move(func_)) {}

Key InputKeyButton::getKey() const {
    return this->key;
}

InputKeyEventType InputKeyButton::getEventType() const {
    return this->eventType;
}

void InputKeyButton::operator()() const {
    this->func();
}

InputMouseButton::InputMouseButton(Key key, InputKeyEventType eventType, std::function<void()> func)
        : InputKeyButton(key, eventType, std::move(func)) {}

InputMouseMovement::InputMouseMovement(InputMouseMovementEventType type_, std::function<void(double,double)> func_)
    : type(type_)
    , func(std::move(func_)) {}

InputMouseMovementEventType InputMouseMovement::getType() const {
    return this->type;
}

void InputMouseMovement::operator()(double x, double y) const {
    this->func(x,y);
}

std::vector<InputKeyButton> InputManager::keyButtonCallbacks;
std::vector<InputMouseButton> InputManager::mouseButtonCallbacks;
std::vector<InputMouseMovement> InputManager::mouseMovementCallbacks;

void InputManager::addCallback(const InputKeyButton& keyButton) {
    InputManager::keyButtonCallbacks.push_back(keyButton);
}

void InputManager::addCallback(const InputMouseButton& mouseButton) {
    InputManager::mouseButtonCallbacks.push_back(mouseButton);
}

void InputManager::addCallback(const InputMouseMovement& mouseMovement) {
    InputManager::mouseMovementCallbacks.push_back(mouseMovement);
}

const std::vector<InputKeyButton>& InputManager::getKeyButtonCallbacks() {
    return InputManager::keyButtonCallbacks;
}

const std::vector<InputMouseButton>& InputManager::getMouseButtonCallbacks() {
    return InputManager::mouseButtonCallbacks;
}

const std::vector<InputMouseMovement>& InputManager::getMouseMovementCallbacks() {
    return InputManager::mouseMovementCallbacks;
}
