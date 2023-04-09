#include "Freecam.h"

#include <config/ConEntry.h>
#include <core/Assertions.h>
#include <core/Engine.h>
#include <input/InputManager.h>

using namespace chira;

bool Freecam::getActive() const {
    return this->active && Device::isMouseCapturedWindow(Engine::getDevice());
}

void Freecam::setActive(bool active_) {
    Device::setMouseCapturedWindow(Engine::getDevice(), active_);
    this->active = active_;
}

void Freecam::setupKeybinds() {
    Input::KeyEvent::create(Input::Key::SDLK_w, Input::KeyEventType::REPEATED, [] {
        if (auto cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, 0, -cam->getMovementSpeed() * (static_cast<float>(Engine::getDeltaTicks()) / 1000)});
    });
    Input::KeyEvent::create(Input::Key::SDLK_s, Input::KeyEventType::REPEATED, [] {
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, 0, cam->getMovementSpeed() * (static_cast<float>(Engine::getDeltaTicks()) / 1000)});
    });
    Input::KeyEvent::create(Input::Key::SDLK_a, Input::KeyEventType::REPEATED, [] {
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({-cam->getMovementSpeed() * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0, 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_d, Input::KeyEventType::REPEATED, [] {
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({cam->getMovementSpeed() * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0, 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_SPACE, Input::KeyEventType::REPEATED, [] {
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, cam->getMovementSpeed() * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_LSHIFT, Input::KeyEventType::REPEATED, [] {
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, -cam->getMovementSpeed() * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_TAB, Input::KeyEventType::PRESSED, [] {
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()))
            cam->setActive(!cam->getActive());
    });
    Input::MouseMotionEvent::create(Input::MouseMotion::MOVEMENT, Input::MouseMotionEventType::NOT_APPLICABLE, [](int, int, int xRel, int yRel) {
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive()) {
            float xOffset = static_cast<float>(xRel) * cam->getMouseSensitivity() * (static_cast<float>(Engine::getDeltaTicks()) / 1000);
            float yOffset = static_cast<float>(yRel) * cam->getMouseSensitivity() * (static_cast<float>(Engine::getDeltaTicks()) / 1000);

            if (auto invertX = ConEntryRegistry::getConVar("input_invert_x_axis"); invertX && invertX->getValue<bool>())
                cam->yaw += xOffset;
            else
                cam->yaw -= xOffset;

            if (auto invertY = ConEntryRegistry::getConVar("input_invert_y_axis"); invertY && invertY->getValue<bool>())
                cam->pitch -= yOffset;
            else
                cam->pitch += yOffset;

            if (cam->pitch > 89.9f)
                cam->pitch = 89.9f;
            else if (cam->pitch < -89.9f)
                cam->pitch = -89.9f;
        }
    });
}
