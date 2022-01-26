#include "meshDataBuilder.h"

#include <algorithm>

using namespace chira;

MeshDataBuilder::MeshDataBuilder() : MeshData() {
    this->drawMode = GL_DYNAMIC_DRAW;
}

void MeshDataBuilder::addVertex(Vertex vertex, bool quickAddDuplicate) { // NOLINT(misc-no-recursion)
    if (quickAddDuplicate) {
        this->vertices.push_back(vertex);
        this->indices.push_back(this->currentIndex);
        this->currentIndex += 1;
        return;
    }
    if (auto position = std::find(this->vertices.begin(), this->vertices.end(), vertex); position != this->vertices.end()) {
        unsigned int index = position - this->vertices.begin();
        this->indices.push_back(index);
    } else {
        this->addVertex(vertex, true);
    }
}

void MeshDataBuilder::update() {
    if (!this->initialized)
        this->setupForRendering();
    this->updateMeshData();
}

void MeshDataBuilder::addTriangle(Vertex v1, Vertex v2, Vertex v3) {
    this->addVertex(v1);
    this->addVertex(v2);
    this->addVertex(v3);
}

void MeshDataBuilder::addSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4) {
    this->addTriangle(v1, v2, v3);
    this->addTriangle(v3, v4, v1);
}
