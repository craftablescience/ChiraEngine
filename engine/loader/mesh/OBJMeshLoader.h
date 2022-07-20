#pragma once

#include "IMeshLoader.h"

namespace chira {

class OBJMeshLoader : public IMeshLoader {
public:
    void loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const override;
    [[nodiscard]] std::vector<byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) const override;
private:
    static void addVertex(Vertex v, unsigned int* currentIndex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
};

} // namespace chira
