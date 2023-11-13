#pragma once

#include "IMeshLoader.h"

namespace chira {

class OBJMeshLoader : public IMeshLoader {
public:
    void loadMesh(const std::string& path, std::vector<Vertex>& vertices, std::vector<Index>& indices) const override;
    [[nodiscard]] std::vector<std::byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) const override;
private:
    static void addVertex(Vertex v, Index* currentIndex, std::vector<Vertex>& vertices, std::vector<Index>& indices);
};

} // namespace chira
