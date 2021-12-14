#pragma once

#include "abstractMeshLoader.h"

namespace chira {
    class objMeshLoader : public abstractMeshLoader {
    public:
        void loadMesh(const std::string& identifier, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) override;
    private:
        static void addVertex(const vertex& v, unsigned int* currentIndex, std::vector<vertex>* vertices, std::vector<unsigned int>* indices);
    };
}
