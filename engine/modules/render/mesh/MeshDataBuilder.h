#pragma once

#include <core/math/Axis.h>
#include <render/mesh/MeshData.h>

namespace chira {

class MeshDataBuilder : public MeshData {
public:
    MeshDataBuilder();

    void addTriangle(Vertex v1, Vertex v2, Vertex v3, bool addDuplicate = false);
    /// Vertex v4 forms a face with vertex v1 and v3.
    void addSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4, bool addDuplicate = false);
    void addSquare(Vertex center, glm::vec2 size, SignedAxis normal, float offset = 0, bool addDuplicate = false);
    void addCube(Vertex center, glm::vec3 size, bool visibleOutside = true, bool addDuplicate = false);
    void update();
    /// Does not call update().
    void clear();

protected:
    Index currentIndex = 0;
    /// Pass true to addDuplicate if you don't want to scan the entire vertex vector to calculate the index.
    /// This will make a duplicate vertex if one already exists.
    void addVertex(Vertex vertex, bool addDuplicate = false);
};

} // namespace chira
