#pragma once

#include "abstractMeshLoader.h"

namespace chira {
    class OBJMeshLoader : public AbstractMeshLoader {
    public:
        void loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) override;
        std::vector<byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) override;
    private:
        static void addVertex(const Vertex& v, unsigned int* currentIndex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    };
}
