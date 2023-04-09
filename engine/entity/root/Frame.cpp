#include "Frame.h"

#include <core/Engine.h>
#include <render/shader/UBO.h>

using namespace chira;

CHIRA_CREATE_LOG(FRAME);

Frame::Frame(std::string name_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Group(std::move(name_))
    , backgroundColor(backgroundColor_)
    , width(width_)
    , height(height_)
    , linearFiltering(smoothResize) {
    this->recreateFramebuffer();
}

Frame::Frame(int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Group()
    , backgroundColor(backgroundColor_)
    , width(width_)
    , height(height_)
    , linearFiltering(smoothResize) {
    this->recreateFramebuffer();
}

void Frame::recreateFramebuffer() {
    if (this->handle) {
        Renderer::destroyFrameBuffer(this->handle);
    }
    this->handle = Renderer::createFrameBuffer(this->width, this->height, WrapMode::REPEAT, WrapMode::REPEAT,
                                               this->linearFiltering ? FilterMode::LINEAR : FilterMode::NEAREST, true);
}

void Frame::render(glm::mat4 /*parentTransform*/) {
    Renderer::setClearColor(ColorRGBA{this->backgroundColor, 1.0f});
    Renderer::pushFrameBuffer(this->handle);

    auto tempPos = this->position;
    this->position = {};
    auto tempRot = this->rotation;
    this->rotation = {};

    // Push camera projection/view
    if (this->mainCamera) {
        PerspectiveViewUBO::get().update(
                this->mainCamera->getProjection(),
                this->mainCamera->getView(),
                this->mainCamera->getGlobalPosition(),
                this->mainCamera->getFrontVector());
    }
    // Push lighting
    this->getLightManager()->updateUBOs();

    Group::render(glm::identity<glm::mat4>());

    if (this->renderSkybox) {
        this->skybox.render(glm::identity<glm::mat4>());
    }

    // Pop camera projection/view
    if (this->mainCamera && Entity::getFrame() && Entity::getFrame()->getCamera()) {
        PerspectiveViewUBO::get().update(
                Entity::getFrame()->getCamera()->getProjection(),
                Entity::getFrame()->getCamera()->getView(),
                Entity::getFrame()->getCamera()->getGlobalPosition(),
                Entity::getFrame()->getCamera()->getFrontVector());
    }
    // Pop lighting
    if (Entity::getFrame() && Entity::getFrame()->getLightManager()) {
        Entity::getFrame()->getLightManager()->updateUBOs();
    }

    // (Hopefully) preserve any transformations from children
    this->translate(tempPos);
    this->rotate(tempRot);

    Renderer::popFrameBuffer();
}

Frame::~Frame() {
    if (this->handle) {
        Renderer::destroyFrameBuffer(this->handle);
    }
}

void Frame::useFrameBufferTexture(TextureUnit activeTextureUnit /*= TextureUnit::G0*/) const {
    Renderer::useFrameBufferTexture(this->handle, activeTextureUnit);
}

glm::vec3 Frame::getGlobalPosition() {
    return this->position;
}

const Frame* Frame::getFrame() const {
    return this;
}

Frame* Frame::getFrame() {
    return this;
}

void Frame::setFrameSize(glm::vec2i newSize) {
    this->width = newSize.x;
    this->height = newSize.y;
    this->recreateFramebuffer();
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

LightManager* Frame::getLightManager() {
    return &this->lightManager;
}

Renderer::FrameBufferHandle* Frame::getRawHandle() {
    return &this->handle;
}
