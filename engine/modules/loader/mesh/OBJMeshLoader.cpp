#include "OBJMeshLoader.h"

#include <algorithm>
#include <sstream>

#include <core/debug/Logger.h>
#include <resource/StringResource.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(OBJ);

void OBJMeshLoader::loadMesh(const std::string& path, std::vector<Vertex>& vertices, std::vector<Index>& indices) const {
    std::vector<glm::vec3> vertexBuffer;
    std::vector<ColorRG> uvBuffer;
    std::vector<ColorRGB> normalBuffer;

    auto meshData = Resource::getResource<StringResource>(path);
    std::istringstream meshDataStream{meshData->getString()};

    std::string line;
    Index currentIndex = 0;
    while (std::getline(meshDataStream, line)) {
        if (line.substr(0,2) == "v ") {
            glm::vec3 pos;
            std::istringstream iss(line.substr(2));
            iss >> pos.x >> pos.y >> pos.z;
            vertexBuffer.push_back(pos);
        } else if (line.substr(0,3) == "vt ") {
            ColorRG uv;
            std::istringstream iss(line.substr(2));
            iss >> uv.r >> uv.g;
            uvBuffer.push_back(uv);
        } else if (line.substr(0,3) == "vn ") {
            ColorRGB normal;
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
                    LOG_OBJ.warning(TRF("warn.obj_loader.not_triangulated", path));
                    break;
                } else if (counter >= 6) {
                    includeUVs = true;
                }
                counter++;
            }
            if (includeUVs) {
                addVertex({{vertexBuffer[objIndices[0]].x, vertexBuffer[objIndices[0]].y, vertexBuffer[objIndices[0]].z},
                           {normalBuffer[objIndices[2]].r, normalBuffer[objIndices[2]].g, normalBuffer[objIndices[2]].b},
                           {uvBuffer[objIndices[1]].r, uvBuffer[objIndices[1]].g}},
                          &currentIndex, vertices, indices);
                addVertex({{vertexBuffer[objIndices[3]].x, vertexBuffer[objIndices[3]].y, vertexBuffer[objIndices[3]].z},
                           {normalBuffer[objIndices[5]].r, normalBuffer[objIndices[5]].g, normalBuffer[objIndices[5]].b},
                           {uvBuffer[objIndices[4]].r, uvBuffer[objIndices[4]].g}},
                          &currentIndex, vertices, indices);
                addVertex({{vertexBuffer[objIndices[6]].x, vertexBuffer[objIndices[6]].y, vertexBuffer[objIndices[6]].z},
                           {normalBuffer[objIndices[8]].r, normalBuffer[objIndices[8]].g, normalBuffer[objIndices[8]].b},
                           {uvBuffer[objIndices[7]].r, uvBuffer[objIndices[7]].g}},
                          &currentIndex, vertices, indices);
            } else {
                addVertex({{vertexBuffer[objIndices[0]].x, vertexBuffer[objIndices[0]].y, vertexBuffer[objIndices[0]].z},
                           {normalBuffer[objIndices[1]].r, normalBuffer[objIndices[1]].g, normalBuffer[objIndices[1]].b}},
                          &currentIndex, vertices, indices);
                addVertex({{vertexBuffer[objIndices[2]].x, vertexBuffer[objIndices[2]].y, vertexBuffer[objIndices[2]].z},
                           {normalBuffer[objIndices[3]].r, normalBuffer[objIndices[3]].g, normalBuffer[objIndices[3]].b}},
                          &currentIndex, vertices, indices);
                addVertex({{vertexBuffer[objIndices[4]].x, vertexBuffer[objIndices[4]].y, vertexBuffer[objIndices[4]].z},
                           {normalBuffer[objIndices[5]].r, normalBuffer[objIndices[5]].g, normalBuffer[objIndices[5]].b}},
                          &currentIndex, vertices, indices);
            }
        }
    }
}

void OBJMeshLoader::addVertex(Vertex v, Index* currentIndex, std::vector<Vertex>& vertices, std::vector<Index>& indices) {
    auto position = std::find(vertices.begin(), vertices.end(), v);
    if (position != vertices.end()) {
        indices.push_back(static_cast<Index>(position - vertices.begin()));
    } else {
        vertices.push_back(v);
        indices.push_back(*currentIndex);
        *currentIndex += 1;
    }
}

std::vector<std::byte> OBJMeshLoader::createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) const {
    std::stringstream meshDataStream;
    meshDataStream.setf(std::stringstream::fixed);
    // The following could be modified to actually use indices and save file space...
    // But it doesn't have to be right now :P
    for (const auto index : indices) {
        meshDataStream << "v " << std::setprecision(6) << vertices[index].position.x << ' ' << vertices[index].position.y << ' ' << vertices[index].position.z << '\n';
        meshDataStream << "vt " << std::setprecision(6) << vertices[index].uv.r << ' ' << vertices[index].uv.g << '\n';
        meshDataStream << "vn " << std::setprecision(6) << vertices[index].normal.r << ' ' << vertices[index].normal.g << ' ' << vertices[index].normal.b << '\n';
    }
    for (Index i = 1; i < indices.size() + 1; i += 3) {
        meshDataStream << "f " << i   << '/' << i   << '/' << i   << ' '
                               << i+1 << '/' << i+1 << '/' << i+1 << ' '
                               << i+2 << '/' << i+2 << '/' << i+2 << '\n';
    }
    meshDataStream >> std::noskipws;

    std::vector<std::byte> out;
    char temp;
    while (meshDataStream >> temp)
        out.push_back(static_cast<std::byte>(temp));
    return out;
}
