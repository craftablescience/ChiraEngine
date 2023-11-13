#pragma once

#include "IMeshLoader.h"

namespace chira {

class ChiraMeshLoader : public IMeshLoader {
public:
    void loadMesh(const std::string& path, std::vector<Vertex>& vertices, std::vector<Index>& indices) const override;
    [[nodiscard]] std::vector<std::byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) const override;
};

struct ChiraMeshHeader {
    unsigned int version = 0;
    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;
};
constexpr unsigned short CHIRA_MESH_HEADER_SIZE = sizeof(ChiraMeshHeader);

} // namespace chira
