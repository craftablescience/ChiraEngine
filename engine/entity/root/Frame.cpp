#include "Frame.h"

#include <core/Engine.h>

using namespace chira;

CHIRA_CREATE_LOG(FRAME);

Frame::Frame(std::string name_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize, bool initNow)
    : Group(std::move(name_))
    , backgroundColor(backgroundColor_)
    , width(width_)
    , height(height_)
    , linearFiltering(smoothResize) {
    if (initNow)
        this->createFramebuffer();
}

Frame::Frame(int width_, int height_, ColorRGB backgroundColor_, bool smoothResize, bool initNow)
    : Group()
    , backgroundColor(backgroundColor_)
    , width(width_)
    , height(height_)
    , linearFiltering(smoothResize) {
    if (initNow)
        this->createFramebuffer();
}

void Frame::createFramebuffer() {
    if (this->fboHandle != 0) {
        glDeleteRenderbuffers(1, &this->rboHandle);
        glDeleteTextures(1, &this->colorTexHandle);
        glDeleteFramebuffers(1, &this->fboHandle);
        this->fboHandle = 0;
        this->colorTexHandle = 0;
        this->rboHandle = 0;
    }

    glGenFramebuffers(1, &this->fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fboHandle);

    glGenTextures(1, &this->colorTexHandle);
    glBindTexture(GL_TEXTURE_2D, this->colorTexHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->linearFiltering ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->linearFiltering ? GL_LINEAR : GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorTexHandle, 0);

    glGenRenderbuffers(1, &this->rboHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rboHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rboHandle);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_FRAME.error("Framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Frame::render(glm::mat4 /*parentTransform*/) {
    this->render(Entity::getFrame()->getFramebufferHandle(), Entity::getFrame()->width, Entity::getFrame()->height);
}

void Frame::render(unsigned int parentFBOHandle, int parentWidth, int parentHeight) {
    glViewport(0, 0, this->width, this->height);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fboHandle);
    glClearColor(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    auto tempPos = this->position;
    this->position = {};
    auto tempRot = this->rotation;
    this->rotation = {};

    // Push camera projection/view
    if (this->mainCamera) {
        UBO_PerspectiveView::get()->update(this->mainCamera->getProjection(), this->mainCamera->getView());
    }

    Group::render(glm::identity<glm::mat4>());

    if (this->renderSkybox) {
        // Wiki says modern hardware is fine with this and it looks better
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        this->skybox.render(glm::identity<glm::mat4>());
    }

    // Pop camera projection/view
    if (this->mainCamera && Entity::getFrame() && Entity::getFrame()->getCamera()) {
        UBO_PerspectiveView::get()->update(Entity::getFrame()->getCamera()->getProjection(), Entity::getFrame()->getCamera()->getView());
    }

    // (Hopefully) preserve any transformations from children
    this->translate(tempPos);
    this->rotate(tempRot);

    glBindFramebuffer(GL_FRAMEBUFFER, parentFBOHandle);
    glViewport(0, 0, parentWidth, parentHeight);
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

void Frame::setFrameSize(glm::vec2i newSize) {
    this->width = newSize.x;
    this->height = newSize.y;
    this->createFramebuffer();
    if (this->getCamera())
        this->getCamera()->createProjection(newSize);
}

glm::vec2i Frame::getFrameSize() const {
    return {this->width, this->height};
}

ColorRGB Frame::getBackgroundColor() const {
    return this->backgroundColor;
}

void Frame::setBackgroundColor(ColorRGB color) {
    this->backgroundColor = color;
}

void Frame::setCamera(Camera* camera) {
    this->mainCamera = camera;
    this->mainCamera->createProjection(this->getFrame()->getFrameSize());
}

Camera* Frame::getCamera() const {
    return this->mainCamera;
}

void Frame::setSkybox(const std::string& cubemapId) {
    if (!this->skyboxMeshCreated) {
        this->skybox.addCube({}, {1, 1, 1}, false);
        this->skybox.update();
        this->skyboxMeshCreated = true;
    }
    this->skybox.setMaterial(Resource::getResource<MaterialCubemap>(cubemapId).castAssert<IMaterial>());
    this->renderSkybox = true;
}

SharedPointer<MaterialCubemap> Frame::getSkybox() const {
    return this->skybox.getMaterial().castAssert<MaterialCubemap>();
}
