#include "MeshData.h"

#include <cstddef>
#include <algorithm>
#include <string>
// todo(render): move to render backend
#include <glad/gl.h>
#include <config/ConEntry.h>
#include <math/Matrix.h>

using namespace chira;

void MeshData::setupForRendering() {
    glGenVertexArrays(1, &this->vaoHandle);
    glGenBuffers(1, &this->vboHandle);
    glGenBuffers(1, &this->eboHandle);

    glBindVertexArray(this->vaoHandle);

    const auto glDrawMode = Renderer::getMeshDrawMode(this->drawMode);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->vertices.size() * sizeof(Vertex)), this->vertices.data(), glDrawMode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->indices.size() * sizeof(Index)), this->indices.data(), glDrawMode);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->initialized = true;
}

void MeshData::updateMeshData() {
    if (!this->initialized)
        return;

    const auto glDrawMode = Renderer::getMeshDrawMode(this->drawMode);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->vertices.size() * sizeof(Vertex)), this->vertices.data(), glDrawMode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->indices.size() * sizeof(Index)), this->indices.data(), glDrawMode);
}

void MeshData::render(glm::mat4 model) {
    if (!this->initialized)
        this->setupForRendering();
    if (this->material) {
        this->material->use();
        if (this->material->getShader()->usesModelMatrix())
            this->material->getShader()->setUniform("m", model);
    }
    glDepthFunc(Renderer::getMeshDepthFunction(this->depthFunction));
    glEnable(GL_CULL_FACE);
    glCullFace(Renderer::getMeshCullType(this->cullType));
    glBindVertexArray(this->vaoHandle);
    glDrawElements(GL_TRIANGLES, static_cast<GLint>(this->indices.size()), GL_UNSIGNED_INT, nullptr);
    glDisable(GL_CULL_FACE);
}

MeshData::~MeshData() {
    if (this->initialized) {
        glDeleteVertexArrays(1, &this->vaoHandle);
        glDeleteBuffers(1, &this->vboHandle);
        glDeleteBuffers(1, &this->eboHandle);
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
