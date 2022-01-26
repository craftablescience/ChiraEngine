#pragma once

#include <render/mesh/meshData.h>
#include <utility/math/axis.h>

namespace chira {
    class MeshDataBuilder : public MeshData {
    public:
        MeshDataBuilder();
        void addTriangle(Vertex v1, Vertex v2, Vertex v3);
        /// Vertex v4 forms a face with vertex v1 and v3.
        void addSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4);
        //todo: implement more mesh builder methods
        //void addSquare(Vertex center, unsigned int width, unsigned int height, SignedAxis normal);
        //void addSquare(Vertex center, unsigned int width, unsigned int height, glm::vec3 rotation);
        //void addCube(Vertex center, unsigned int width, unsigned int height, unsigned int depth);
        void update();
    protected:
        unsigned int currentIndex = 0;
        /// Pass true to quickAddDuplicate if you don't want to scan the entire vertex vector to calculate the index.
        /// This will make a duplicate vertex if one already exists.
        void addVertex(Vertex vertex, bool quickAddDuplicate = false);
    };
}
