#include "window.h"

#include <core/engine.h> //todo(viewport): remove me

#include <render/material/materialFramebuffer.h>

using namespace chira;

Window::Window(const std::string& name_, const std::string& title_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Frame(name_, width_, height_, backgroundColor_, smoothResize, false) {
    //todo(viewport): init GLFW and ImGUI
    this->createFramebuffer(smoothResize);
    this->surface.enableBackfaceCulling(false);
    this->surface.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    this->surface.setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/window.json", this).castAssert<MaterialBase>());
}

Window::Window(const std::string& title_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Frame(width_, height_, backgroundColor_, smoothResize, false) {
    //todo(viewport): init GLFW and ImGUI
    this->createFramebuffer(smoothResize);
    this->surface.enableBackfaceCulling(false);
    this->surface.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    this->surface.setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/window.json", this).castAssert<MaterialBase>());
}

void Window::render(glm::mat4 parentTransform) {
    Frame::render(std::forward<glm::mat4>(parentTransform), 0, this->width, this->height);
    glDisable(GL_DEPTH_TEST);
    this->surface.render(glm::identity<glm::mat4>());
}

Window::~Window() = default; // {
    //todo(viewport): de-init GLFW and ImGUI
//}

const Window* Window::getWindow() const {
    return this;
}

Window* Window::getWindow() {
    return this;
}

glm::vec2 Window::getMousePosition() const {
    return Engine::getMousePosition(); //todo(viewport)
}
