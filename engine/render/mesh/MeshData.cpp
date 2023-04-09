#include "MeshData.h"

#include <algorithm>
#include <string>
#include <config/ConEntry.h>
#include <math/Matrix.h>

using namespace chira;

void MeshData::setupForRendering() {
    this->handle = Renderer::createMesh(this->vertices, this->indices, MeshDrawMode::STATIC);
    this->initialized = true;
}

void MeshData::updateMeshData() {
    if (!this->initialized)
        return;
    Renderer::updateMesh(&this->handle, this->vertices, this->indices, this->drawMode);
}

void MeshData::render(glm::mat4 model) {
    if (!this->initialized)
        this->setupForRendering();
    if (this->material) {
        this->material->use();
        if (this->material->getShader()->usesModelMatrix())
            this->material->getShader()->setUniform("m", model);
    }
    Renderer::drawMesh(this->handle, this->depthFunction, this->cullType);
}

MeshData::~MeshData() {
    if (this->initialized) {
        Renderer::destroyMesh(this->handle);
    }
}

SharedPointer<IMaterial> MeshData::getMaterial() const {
    return this->material;
}

void MeshData::setMaterial(SharedPointer<IMaterial> newMaterial) {
    this->material = std::move(newMaterial);
}

MeshDepthFunction MeshData::getDepthFunction() const {
    return this->depthFunction;
}

void MeshData::setDepthFunction(MeshDepthFunction function) {
    this->depthFunction = function;
}

MeshCullType MeshData::getCullType() const {
    return this->cullType;
}

void MeshData::setCullType(MeshCullType type) {
    this->cullType = type;
}

std::vector<byte> MeshData::getMeshData(const std::string& meshLoader) const {
    return IMeshLoader::getMeshLoader(meshLoader)->createMesh(this->vertices, this->indices);
}

void MeshData::appendMeshData(const std::string& loader, const std::string& identifier) {
    IMeshLoader::getMeshLoader(loader)->loadMesh(identifier, this->vertices, this->indices);
}

void MeshData::clearMeshData() {
    this->vertices.clear();
    this->indices.clear();
}
