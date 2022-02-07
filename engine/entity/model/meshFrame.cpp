#include "meshFrame.h"

#include <render/material/materialFramebuffer.h>

using namespace chira;

MeshFrame::MeshFrame(const std::string& name_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Frame(name_, width_, height_, backgroundColor_, smoothResize) {
    this->mesh.getMesh()->setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/unlitTextured.json", this).castAssert<MaterialBase>());
}

MeshFrame::MeshFrame(int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Frame(width_, height_, backgroundColor_, smoothResize) {
    this->mesh.getMesh()->setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/unlitTextured.json", this).castAssert<MaterialBase>());
}

void MeshFrame::render(glm::mat4 parentTransform) {
    Frame::render(parentTransform);
    this->mesh.render(transformToMatrix(parentTransform, this->position, this->rotation));
}

MeshDynamic* MeshFrame::getMeshDynamic() {
    return &this->mesh;
}
