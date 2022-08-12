#include "ChiraMeshLoader.h"

#include <cstring>
#include <algorithm>
#include <core/Logger.h>
#include <resource/BinaryResource.h>
#include <i18n/TranslationManager.h>

using namespace chira;

void ChiraMeshLoader::loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<Index>& indices) const {
    auto meshData = Resource::getResource<BinaryResource>(identifier);
    if (meshData->getBufferLength() < CHIRA_MESH_HEADER_SIZE) {
        // die
        Logger::log(LogType::LOG_ERROR, "CMDL", TRF("error.cmdl_loader.invalid_data", identifier));
        return;
    }
    ChiraMeshHeader header;
    std::memcpy(&header, meshData->getBuffer(), CHIRA_MESH_HEADER_SIZE);

    // read mesh data
    if (header.version == 1) {
        vertices.resize(header.vertexCount);
        std::memcpy(&vertices[0], meshData->getBuffer() + CHIRA_MESH_HEADER_SIZE, header.vertexCount * sizeof(Vertex));
        indices.resize(header.indexCount);
        std::memcpy(&indices[0], meshData->getBuffer() + CHIRA_MESH_HEADER_SIZE + (header.vertexCount * sizeof(Vertex)), header.indexCount * sizeof(Index));
    }
}

std::vector<byte> ChiraMeshLoader::createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) const {
    std::vector<byte> bytebuffer;
    ChiraMeshHeader header;
    header.version = 1;
    header.vertexCount = static_cast<Index>(vertices.size());
    header.indexCount = static_cast<Index>(indices.size());
    const unsigned int VERTEX_SIZE = (sizeof(Vertex) * header.vertexCount);
    const unsigned int INDEX_SIZE = (sizeof(Index) * header.indexCount);

    // Since each element in bytebuffer is one byte wide, this works
    bytebuffer.resize(CHIRA_MESH_HEADER_SIZE + VERTEX_SIZE + INDEX_SIZE);
    std::memcpy(&bytebuffer[0], &header, CHIRA_MESH_HEADER_SIZE);
    std::memcpy(&bytebuffer[CHIRA_MESH_HEADER_SIZE], &vertices[0], VERTEX_SIZE);
    std::memcpy(&bytebuffer[CHIRA_MESH_HEADER_SIZE + VERTEX_SIZE], &indices[0], INDEX_SIZE);

    return bytebuffer;
}
