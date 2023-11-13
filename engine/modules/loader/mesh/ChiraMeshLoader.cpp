#include "ChiraMeshLoader.h"

#include <cstring>
#include <algorithm>

#include <core/debug/Logger.h>
#include <resource/BinaryResource.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(CMDL);

void ChiraMeshLoader::loadMesh(const std::string& path, std::vector<Vertex>& vertices, std::vector<Index>& indices) const {
    auto meshData = Resource::getResource<BinaryResource>(path);
    if (meshData->getBufferLength() < CHIRA_MESH_HEADER_SIZE) {
        // die
        LOG_CMDL.error(TRF("error.cmdl_loader.invalid_data", path));
        return;
    }
    ChiraMeshHeader header;
    std::memcpy(&header, meshData->getBuffer(), CHIRA_MESH_HEADER_SIZE);

    // read mesh data
    if (header.version == 1) {
        vertices.resize(header.vertexCount);
        std::memcpy(vertices.data(), meshData->getBuffer() + CHIRA_MESH_HEADER_SIZE, header.vertexCount * sizeof(Vertex));
        indices.resize(header.indexCount);
        std::memcpy(indices.data(), meshData->getBuffer() + CHIRA_MESH_HEADER_SIZE + (header.vertexCount * sizeof(Vertex)), header.indexCount * sizeof(Index));
    }
}

std::vector<std::byte> ChiraMeshLoader::createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) const {
    std::vector<std::byte> bytebuffer;
    ChiraMeshHeader header;
    header.version = 1;
    header.vertexCount = static_cast<Index>(vertices.size());
    header.indexCount = static_cast<Index>(indices.size());
    const unsigned int VERTEX_SIZE = (sizeof(Vertex) * header.vertexCount);
    const unsigned int INDEX_SIZE = (sizeof(Index) * header.indexCount);

    // Since each element in bytebuffer is one byte wide, this works
    bytebuffer.resize(CHIRA_MESH_HEADER_SIZE + VERTEX_SIZE + INDEX_SIZE);
    std::memcpy(bytebuffer.data(), &header, CHIRA_MESH_HEADER_SIZE);
    std::memcpy(bytebuffer.data() + CHIRA_MESH_HEADER_SIZE, vertices.data(), VERTEX_SIZE);
    std::memcpy(bytebuffer.data() + CHIRA_MESH_HEADER_SIZE + VERTEX_SIZE, indices.data(), INDEX_SIZE);

    return bytebuffer;
}
