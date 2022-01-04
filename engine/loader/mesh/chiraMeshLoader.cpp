#include "chiraMeshLoader.h"

#include <algorithm>
#include <resource/binaryResource.h>
#include <i18n/translationManager.h>
#include <utility/logger.h>

using namespace chira;

void ChiraMeshLoader::loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    auto meshData = Resource::getResource<BinaryResource>(identifier);
    if (meshData->getBufferLength() < CHIRA_MESH_HEADER_SIZE) {
        // die
        Logger::log(LogType::ERROR, "CMDL", fmt::format(TR("error.cmdl_loader.invalid_data"), identifier));
        return;
    }
    ChiraMeshHeader header;
    memcpy(&header, meshData->getBuffer(), CHIRA_MESH_HEADER_SIZE);

    // read mesh data
    if (header.version == 1) {
        vertices.resize(header.vertexCount);
        memcpy(&vertices[0], meshData->getBuffer() + CHIRA_MESH_HEADER_SIZE, header.vertexCount * sizeof(Vertex));
        indices.resize(header.indexCount);
        memcpy(&indices[0], meshData->getBuffer() + CHIRA_MESH_HEADER_SIZE + (header.vertexCount * sizeof(Vertex)), header.indexCount * sizeof(unsigned int));
    }
}

std::vector<byte> ChiraMeshLoader::createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    std::vector<byte> bytebuffer;
    ChiraMeshHeader header;
    header.version = 1;
    header.vertexCount = vertices.size();
    header.indexCount = indices.size();
    const unsigned int VERTEX_SIZE = (sizeof(Vertex) * header.vertexCount);
    const unsigned int INDEX_SIZE = (sizeof(unsigned int) * header.indexCount);

    // Since each element in bytebuffer is one byte wide, this works
    bytebuffer.resize(CHIRA_MESH_HEADER_SIZE + VERTEX_SIZE + INDEX_SIZE);
    memcpy(&bytebuffer[0], &header, CHIRA_MESH_HEADER_SIZE);
    memcpy(&bytebuffer[CHIRA_MESH_HEADER_SIZE], &vertices[0], VERTEX_SIZE);
    memcpy(&bytebuffer[CHIRA_MESH_HEADER_SIZE + VERTEX_SIZE], &indices[0], INDEX_SIZE);

    return bytebuffer;
}
