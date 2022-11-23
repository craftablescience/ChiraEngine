#include "MeshDataBuilder.h"

#include <algorithm>

using namespace chira;

MeshDataBuilder::MeshDataBuilder() : MeshData() {
    this->drawMode = MeshDrawMode::DYNAMIC;
}

void MeshDataBuilder::addVertex(Vertex vertex, bool addDuplicate) { // NOLINT(misc-no-recursion)
    if (addDuplicate) {
        this->vertices.push_back(vertex);
        this->indices.push_back(this->currentIndex);
        this->currentIndex += 1;
        return;
    }
    if (auto position = std::find(this->vertices.begin(), this->vertices.end(), vertex); position != this->vertices.end()) {
        this->indices.push_back(static_cast<Index>(position - this->vertices.begin()));
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

void MeshDataBuilder::addSquare(Vertex center, glm::vec2 size, SignedAxis normal, float offset, bool addDuplicate) {
    const float s1 = size.x / 2;
    const float s2 = size.y / 2;
    const glm::vec3 mid{center.position.x, center.position.y, center.position.z};
    const glm::vec3 col{center.color.r,    center.color.g,    center.color.b};

    switch (normal) {
        using enum SignedAxis;
        case YP:
            this->addSquare(
                    {{mid.x - s1, mid.y + offset, mid.z + s2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                    {{mid.x + s1, mid.y + offset, mid.z + s2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                    {{mid.x + s1, mid.y + offset, mid.z - s2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                    {{mid.x - s1, mid.y + offset, mid.z - s2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                    addDuplicate
            );
            break;
        case YN:
            this->addSquare(
                    {{mid.x - s1, mid.y - offset, mid.z - s2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                    {{mid.x + s1, mid.y - offset, mid.z - s2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                    {{mid.x + s1, mid.y - offset, mid.z + s2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                    {{mid.x - s1, mid.y - offset, mid.z + s2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                    addDuplicate
            );
            break;
        case ZP:
            this->addSquare(
                    {{mid.x - s1, mid.y + s2, mid.z + offset}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                    {{mid.x - s1, mid.y - s2, mid.z + offset}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                    {{mid.x + s1, mid.y - s2, mid.z + offset}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                    {{mid.x + s1, mid.y + s2, mid.z + offset}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                    addDuplicate
            );
            break;
        case ZN:
            this->addSquare(
                    {{mid.x + s1, mid.y + s2, mid.z - offset}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                    {{mid.x + s1, mid.y - s2, mid.z - offset}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                    {{mid.x - s1, mid.y - s2, mid.z - offset}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                    {{mid.x - s1, mid.y + s2, mid.z - offset}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                    addDuplicate
            );
            break;
        case XP:
            this->addSquare(
                    {{mid.x + offset, mid.y + s1, mid.z + s2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                    {{mid.x + offset, mid.y - s1, mid.z + s2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                    {{mid.x + offset, mid.y - s1, mid.z - s2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                    {{mid.x + offset, mid.y + s1, mid.z - s2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                    addDuplicate
            );
            break;
        case XN:
            this->addSquare(
                    {{mid.x - offset, mid.y - s1, mid.z - s2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                    {{mid.x - offset, mid.y - s1, mid.z + s2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                    {{mid.x - offset, mid.y + s1, mid.z + s2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                    {{mid.x - offset, mid.y + s1, mid.z - s2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                    addDuplicate
            );
            break;
    }
}

void MeshDataBuilder::addCube(Vertex center, glm::vec3 size, bool visibleOutside, bool addDuplicate) {
    const float x2 = size.x / 2;
    const float y2 = size.y / 2;
    const float z2 = size.z / 2;
    const glm::vec3 mid{center.position.x, center.position.y, center.position.z};
    const glm::vec3 col{center.color.r,    center.color.g,    center.color.b};

    if (visibleOutside) {
        this->addSquare(
                {{mid.x - x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                {{mid.x + x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                {{mid.x + x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x - x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x - x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                {{mid.x + x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                {{mid.x + x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x - x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x - x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                {{mid.x - x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x + x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x + x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x + x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                {{mid.x + x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x - x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x - x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x + x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                {{mid.x + x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x + x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x + x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x - x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x - x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x - x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                {{mid.x - x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                addDuplicate
        );
    } else {
        this->addSquare(
                {{mid.x - x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x + x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x + x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                {{mid.x - x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x - x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x + x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x + x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                {{mid.x - x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x + x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                {{mid.x + x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x - x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x - x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x - x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                {{mid.x - x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x + x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x + x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x + x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                {{mid.x + x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                {{mid.x + x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x + x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                addDuplicate
        );
        this->addSquare(
                {{mid.x - x2, mid.y + y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 0}},
                {{mid.x - x2, mid.y + y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 0}},
                {{mid.x - x2, mid.y - y2, mid.z + z2}, {0, 0, 0}, {col.x, col.y, col.z}, {1, 1}},
                {{mid.x - x2, mid.y - y2, mid.z - z2}, {0, 0, 0}, {col.x, col.y, col.z}, {0, 1}},
                addDuplicate
        );
    }
}

void MeshDataBuilder::update() {
    if (!this->initialized)
        this->setupForRendering();
    this->updateMeshData();
}

void MeshDataBuilder::clear() {
    this->clearMeshData();
}
