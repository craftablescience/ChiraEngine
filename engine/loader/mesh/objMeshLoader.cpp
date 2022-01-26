#include "objMeshLoader.h"

#include <algorithm>
#include <resource/stringResource.h>
#include <i18n/translationManager.h>
#include <utility/logger.h>
#include <sstream>

using namespace chira;

void OBJMeshLoader::loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const {
    std::vector<Position> vertexBuffer;
    std::vector<UV> uvBuffer;
    std::vector<Normal> normalBuffer;

    auto meshData = Resource::getResource<StringResource>(identifier);
    std::istringstream meshDataStream{meshData->getString()};

    std::string line;
    unsigned int currentIndex = 0;
    while (std::getline(meshDataStream, line)) {
        if (line.substr(0,2) == "v ") {
            Position pos;
            std::istringstream iss(line.substr(2));
            iss >> pos.x >> pos.y >> pos.z;
            vertexBuffer.push_back(pos);
        } else if (line.substr(0,3) == "vt ") {
            UV uv;
            std::istringstream iss(line.substr(2));
            iss >> uv.u >> uv.v;
            uvBuffer.push_back(uv);
        } else if (line.substr(0,3) == "vn ") {
            Normal normal;
            std::istringstream iss(line.substr(2));
            iss >> normal.r >> normal.g >> normal.b;
            normalBuffer.push_back(normal);
        } else if (line.substr(0,2) == "f ") {
            // this line has 10 zeroes to check if OBJ is triangulated concisely
            int objIndices[10];
            std::string data = line.substr(2);
            std::replace(data.begin(), data.end(), '/', ' ');
            std::istringstream iss(data);
            unsigned short counter = 0;
            bool includeUVs = false;
            while (iss >> objIndices[counter]) {
                objIndices[counter] -= 1;
                if (counter >= 9) {
                    Logger::log(LogType::WARNING, "OBJ", TRF("warn.obj_loader.not_triangulated", identifier));
                    break;
                } else if (counter >= 6) {
                    includeUVs = true;
                }
                counter++;
            }
            if (includeUVs) {
                addVertex(Vertex(vertexBuffer[objIndices[0]].x, vertexBuffer[objIndices[0]].y, vertexBuffer[objIndices[0]].z,
                                 normalBuffer[objIndices[2]].r, normalBuffer[objIndices[2]].g, normalBuffer[objIndices[2]].b,
                                 uvBuffer[objIndices[1]].u, uvBuffer[objIndices[1]].v), &currentIndex, vertices, indices);
                addVertex(Vertex(vertexBuffer[objIndices[3]].x, vertexBuffer[objIndices[3]].y, vertexBuffer[objIndices[3]].z,
                                 normalBuffer[objIndices[5]].r, normalBuffer[objIndices[5]].g, normalBuffer[objIndices[5]].b,
                                 uvBuffer[objIndices[4]].u, uvBuffer[objIndices[4]].v), &currentIndex, vertices, indices);
                addVertex(Vertex(vertexBuffer[objIndices[6]].x, vertexBuffer[objIndices[6]].y, vertexBuffer[objIndices[6]].z,
                                 normalBuffer[objIndices[8]].r, normalBuffer[objIndices[8]].g, normalBuffer[objIndices[8]].b,
                                 uvBuffer[objIndices[7]].u, uvBuffer[objIndices[7]].v), &currentIndex, vertices, indices);
            } else {
                addVertex(Vertex(vertexBuffer[objIndices[0]].x, vertexBuffer[objIndices[0]].y, vertexBuffer[objIndices[0]].z,
                                 normalBuffer[objIndices[1]].r, normalBuffer[objIndices[1]].g, normalBuffer[objIndices[1]].b),
                                 &currentIndex, vertices, indices);
                addVertex(Vertex(vertexBuffer[objIndices[2]].x, vertexBuffer[objIndices[2]].y, vertexBuffer[objIndices[2]].z,
                                 normalBuffer[objIndices[3]].r, normalBuffer[objIndices[3]].g, normalBuffer[objIndices[3]].b),
                                 &currentIndex, vertices, indices);
                addVertex(Vertex(vertexBuffer[objIndices[4]].x, vertexBuffer[objIndices[4]].y, vertexBuffer[objIndices[4]].z,
                                 normalBuffer[objIndices[5]].r, normalBuffer[objIndices[5]].g, normalBuffer[objIndices[5]].b),
                                 &currentIndex, vertices, indices);
            }
        }
    }
}

void OBJMeshLoader::addVertex(Vertex v, unsigned int* currentIndex, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    auto position = std::find(vertices.begin(), vertices.end(), v);
    if (position != vertices.end()) {
        unsigned int index = position - vertices.begin();
        indices.push_back(index);
    } else {
        vertices.push_back(v);
        indices.push_back(*currentIndex);
        *currentIndex += 1;
    }
}

std::vector<byte> OBJMeshLoader::createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) const {
    std::vector<Position> positions;
    std::vector<UV> uvs;
    std::vector<Normal> normals;
    positions.reserve(indices.size());
    uvs.reserve(indices.size());
    normals.reserve(indices.size());

    for (const auto index : indices) {
        positions.push_back(vertices[index].position);
        uvs.push_back(vertices[index].uv);
        normals.push_back(vertices[index].normal);
    }
    std::stringstream meshDataStream;
    // The following could be modified to actually use indices and save file space...
    // But it doesn't have to be right now :P
    for (const auto position : positions) meshDataStream << "v " << position.x << ' ' << position.y << ' ' << position.z << '\n';
    for (const auto uv       : uvs)       meshDataStream << "vt " << uv.u << ' ' << uv.v << '\n';
    for (const auto normal   : normals)   meshDataStream << "vn " << normal.r << ' ' << normal.g << ' ' << normal.b << '\n';
    for (unsigned int i = 0; i < vertices.size(); i += 3) {
        meshDataStream << "f " << i   << '/' << i   << '/' << i   << ' '
                               << i+1 << '/' << i+1 << '/' << i+1 << ' '
                               << i+2 << '/' << i+2 << '/' << i+2 << '\n';
    }
    meshDataStream >> std::noskipws;

    std::vector<byte> out;
    byte temp;
    while (meshDataStream >> temp)
        out.push_back(temp);
    return out;
}
