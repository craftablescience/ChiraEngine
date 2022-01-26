#pragma once

#include <render/mesh/meshData.h>
#include <utility/math/axis.h>

namespace chira {
    class MeshDataBuilder : public MeshData {
    public:
        MeshDataBuilder();
        void addTriangle(Vertex v1, Vertex v2, Vertex v3, bool addDuplicate = false);
        /// Vertex v4 forms a face with vertex v1 and v3.
        void addSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4, bool addDuplicate = false);
        //todo: implement more mesh builder methods
        //void addSquare(glm::vec3 center, unsigned int width, unsigned int height, SignedAxis normal, int offset = 0, bool addDuplicate = false);
        //void addSquare(Vertex center, unsigned int width, unsigned int height, glm::vec3 rotation, bool addDuplicate = false);
        void addCube(glm::vec3 center, unsigned int x, unsigned int y, unsigned int z, bool visibleOutside = true, bool addDuplicate = false);
        void enableBackfaceCulling(bool backfaceCulling_);
        [[nodiscard]] bool isBackfaceCullingEnabled() const;
        void update();
    protected:
        unsigned int currentIndex = 0;
        /// Pass true to quickAddDuplicate if you don't want to scan the entire vertex vector to calculate the index.
        /// This will make a duplicate vertex if one already exists.
        void addVertex(Vertex vertex, bool addDuplicate = false);
    };
}
