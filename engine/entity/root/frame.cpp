#include "frame.h"

#include <core/engine.h>

using namespace chira;

Frame::Frame(const std::string& name_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Root(name_)
    , backgroundColor(backgroundColor_)
    , width(width_)
    , height(height_) {
    createFramebuffer(smoothResize);
}

Frame::Frame(int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Root()
    , backgroundColor(backgroundColor_)
    , width(width_)
    , height(height_) {
    createFramebuffer(smoothResize);
}

void Frame::createFramebuffer(bool smoothResize) {
    glGenFramebuffers(1, &this->fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fboHandle);

    glGenTextures(1, &this->colorTexHandle);
    glBindTexture(GL_TEXTURE_2D, this->colorTexHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smoothResize ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smoothResize ? GL_LINEAR : GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTexHandle, 0);

    glGenRenderbuffers(1, &this->rboHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rboHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rboHandle);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::log(LogType::ERROR, "Frame", "Framebuffer is not complete!"); // todo(i18n)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Frame::render(glm::mat4 parentTransform) {
    glViewport(0, 0, this->width, this->height);
    glClearColor(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    Root::render(std::forward<glm::mat4>(parentTransform));

    glBindFramebuffer(GL_FRAMEBUFFER, Entity::getFrame()->getFramebufferHandle());
    glViewport(0, 0, Entity::getFrame()->width, Entity::getFrame()->height);
}

Frame::~Frame() {
    glDeleteRenderbuffers(1, &this->rboHandle);
    glDeleteTextures(1, &this->colorTexHandle);
    glDeleteFramebuffers(1, &this->fboHandle);
}

const Frame* Frame::getFrame() const {
    return this;
}

Frame* Frame::getFrame() {
    return this;
}

unsigned int Frame::getFramebufferHandle() const {
    return this->fboHandle;
}

unsigned int Frame::getColorTextureHandle() const {
    return this->colorTexHandle;
}

glm::vec2 Frame::getFrameSize() const {
    return {this->width, this->height};
}

ColorRGB Frame::getBackgroundColor() const {
    return this->backgroundColor;
}

void Frame::setBackgroundColor(ColorRGB color) {
    this->backgroundColor = color;
}