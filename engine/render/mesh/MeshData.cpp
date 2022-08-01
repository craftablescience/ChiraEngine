#include "MeshData.h"

#include <cstddef>
#include <algorithm>

#include <math/Matrix.h>

using namespace chira;

void MeshData::setupForRendering() {
    glGenVertexArrays(1, &this->vaoHandle);
    glGenBuffers(1, &this->vboHandle);
    glGenBuffers(1, &this->eboHandle);

    glBindVertexArray(this->vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->vertices.size() * sizeof(Vertex)), &this->vertices[0], this->drawMode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->indices.size() * sizeof(unsigned int)), &this->indices[0], this->drawMode);

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
    glNamedBufferData(this->vboHandle, static_cast<GLsizeiptr>(this->vertices.size() * sizeof(Vertex)), &this->vertices[0], this->drawMode);
    glNamedBufferData(this->eboHandle, static_cast<GLsizeiptr>(this->indices.size() * sizeof(unsigned int)), &this->indices[0], this->drawMode);
}

void MeshData::render(glm::mat4 model) {
    if (!this->initialized)
        this->setupForRendering();
    if (this->material) {
        this->material->use();
        if (this->material->getShader()->usesModelMatrix())
            this->material->getShader()->setUniform("m", model);
    }
    glDepthFunc(this->depthFunction);
    if (this->backfaceCulling) {
        glEnable(GL_CULL_FACE);
        glCullFace(this->cullType);
    } else {
        glDisable(GL_CULL_FACE);
    }
    glBindVertexArray(this->vaoHandle);
    glDrawElements(GL_TRIANGLES, static_cast<GLint>(this->indices.size()), GL_UNSIGNED_INT, nullptr);
}

MeshData::~MeshData() {
    if (this->initialized) {
        glDeleteVertexArrays(1, &this->vaoHandle);
        glDeleteBuffers(1, &this->vboHandle);
        glDeleteBuffers(1, &this->eboHandle);
    }
}

SharedPointer<MaterialBase> MeshData::getMaterial() const {
    return this->material;
}

void MeshData::setMaterial(SharedPointer<MaterialBase> newMaterial) {
    this->material = std::move(newMaterial);
}

int MeshData::getDepthFunction() const {
    return this->depthFunction;
}

int MeshData::getCullType() const {
    return this->cullType;
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
