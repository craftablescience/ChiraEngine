#include "meshDataBuilder.h"

#include <algorithm>

using namespace chira;

MeshDataBuilder::MeshDataBuilder() : MeshData() {
    this->drawMode = GL_DYNAMIC_DRAW;
}

void MeshDataBuilder::addVertex(Vertex vertex, bool addDuplicate) { // NOLINT(misc-no-recursion)
    if (addDuplicate) {
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

void MeshDataBuilder::addTriangle(Vertex v1, Vertex v2, Vertex v3, bool addDuplicate) {
    this->addVertex(v1, addDuplicate);
    this->addVertex(v2, addDuplicate);
    this->addVertex(v3, addDuplicate);
}

void MeshDataBuilder::addSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4, bool addDuplicate) {
    this->addTriangle(v1, v2, v3, addDuplicate);
    this->addTriangle(v3, v4, v1, addDuplicate);
}

void MeshDataBuilder::addCube(Vertex center, glm::vec3 size, bool visibleOutside, bool addDuplicate) {
    const float x2 = size.x / 2;
    const float y2 = size.y / 2;
    const float z2 = size.z / 2;
    const glm::vec3 mid{center.position.x, center.position.y, center.position.z};
    const glm::vec3 col{center.color.r,    center.color.g,    center.color.b};

    if (visibleOutside) {
        this->addSquare(
                {mid.x - x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                {mid.x + x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                {mid.x + x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x - x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                addDuplicate
        );
        this->addSquare(
                {mid.x - x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                {mid.x + x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                {mid.x + x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x - x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                addDuplicate
        );
        this->addSquare(
                {mid.x - x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                {mid.x - x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x + x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x + x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                addDuplicate
        );
        this->addSquare(
                {mid.x + x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                {mid.x + x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x - x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x - x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                addDuplicate
        );
        this->addSquare(
                {mid.x + x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                {mid.x + x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x + x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x + x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                addDuplicate
        );
        this->addSquare(
                {mid.x - x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x - x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x - x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                {mid.x - x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                addDuplicate
        );
    } else {
        this->addSquare(
                {mid.x - x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x + x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x + x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                {mid.x - x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                addDuplicate
        );
        this->addSquare(
                {mid.x - x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x + x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x + x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                {mid.x - x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                addDuplicate
        );
        this->addSquare(
                {mid.x + x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                {mid.x + x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x - x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x - x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                addDuplicate
        );
        this->addSquare(
                {mid.x - x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                {mid.x - x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x + x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x + x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                addDuplicate
        );
        this->addSquare(
                {mid.x + x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                {mid.x + x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                {mid.x + x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x + x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                addDuplicate
        );
        this->addSquare(
                {mid.x - x2, mid.y + y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 0},
                {mid.x - x2, mid.y + y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 0},
                {mid.x - x2, mid.y - y2, mid.z + z2, 0, 0, 0, col.x, col.y, col.z, 1, 1},
                {mid.x - x2, mid.y - y2, mid.z - z2, 0, 0, 0, col.x, col.y, col.z, 0, 1},
                addDuplicate
        );
    }
}

void MeshDataBuilder::enableBackfaceCulling(bool backfaceCulling_) {
    this->backfaceCulling = backfaceCulling_;
}

bool MeshDataBuilder::isBackfaceCullingEnabled() const {
    return this->backfaceCulling;
}

void MeshDataBuilder::update() {
    if (!this->initialized)
        this->setupForRendering();
    this->updateMeshData();
}

void MeshDataBuilder::clear() {
    this->clearMeshData();
}
