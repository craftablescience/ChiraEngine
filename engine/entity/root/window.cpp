#include "window.h"

using namespace chira;

Window::Window(const std::string& name_, const std::string& title_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Frame(name_, width_, height_, backgroundColor_, smoothResize, false) {
    //todo(viewport): init GLFW and ImGUI
    this->createFramebuffer(smoothResize);
}

Window::Window(const std::string& title_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Frame(width_, height_, backgroundColor_, smoothResize, false) {
    //todo(viewport): init GLFW and ImGUI
    this->createFramebuffer(smoothResize);
}

void Window::render(glm::mat4 parentTransform) {
    //todo(viewport): use framebuffer texture to render onto a square mesh (see splashscreen)
    Frame::render(parentTransform);
}

Window::~Window() {
    //todo(viewport): de-init GLFW and ImGUI
}

const Window* Window::getWindow() const {
    return this;
}

Window* Window::getWindow() {
    return this;
}
