#include <fstream>
#include <algorithm>
#include "objMeshLoader.h"
#include "../core/engine.h"

void objMeshLoader::loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) {
    std::vector<position> vertexBuffer;
    std::vector<uv> uvBuffer;
    std::vector<normal> normalBuffer;

    std::ifstream input(virtualFileSystem::getMeshPath(filepath));
    std::string line;
    unsigned int currentIndex = 0;
    while (std::getline(input, line)) {
        if (line.substr(0,2) == "v ") {
            position pos;
            std::istringstream iss(line.substr(2));
            iss >> pos.x >> pos.y >> pos.z;
            vertexBuffer.push_back(pos);
        } else if (line.substr(0,3) == "vt ") {
            uv uv;
            std::istringstream iss(line.substr(2));
            iss >> uv.u >> uv.v;
            uvBuffer.push_back(uv);
        } else if (line.substr(0,3) == "vn ") {
            normal normal;
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
                    chiraLogger::log(WARN, "OBJ", "OBJ file " + filepath + " is not triangulated");
                    break;
                } else if (counter >= 6) {
                    includeUVs = true;
                }
                counter++;
            }
            if (includeUVs) {
                addVertex(vertex(vertexBuffer[objIndices[0]].x, vertexBuffer[objIndices[0]].y, vertexBuffer[objIndices[0]].z,
                                 normalBuffer[objIndices[2]].r, normalBuffer[objIndices[2]].g, normalBuffer[objIndices[2]].b,
                                 uvBuffer[objIndices[1]].u, uvBuffer[objIndices[1]].v), &currentIndex, vertices, indices);
                addVertex(vertex(vertexBuffer[objIndices[3]].x, vertexBuffer[objIndices[3]].y, vertexBuffer[objIndices[3]].z,
                                 normalBuffer[objIndices[5]].r, normalBuffer[objIndices[5]].g, normalBuffer[objIndices[5]].b,
                                 uvBuffer[objIndices[4]].u, uvBuffer[objIndices[4]].v), &currentIndex, vertices, indices);
                addVertex(vertex(vertexBuffer[objIndices[6]].x, vertexBuffer[objIndices[6]].y, vertexBuffer[objIndices[6]].z,
                                 normalBuffer[objIndices[8]].r, normalBuffer[objIndices[8]].g, normalBuffer[objIndices[8]].b,
                                 uvBuffer[objIndices[7]].u, uvBuffer[objIndices[7]].v), &currentIndex, vertices, indices);
            } else {
                addVertex(vertex(vertexBuffer[objIndices[0]].x, vertexBuffer[objIndices[0]].y, vertexBuffer[objIndices[0]].z,
                                 normalBuffer[objIndices[1]].r, normalBuffer[objIndices[1]].g, normalBuffer[objIndices[1]].b),
                                 &currentIndex, vertices, indices);
                addVertex(vertex(vertexBuffer[objIndices[2]].x, vertexBuffer[objIndices[2]].y, vertexBuffer[objIndices[2]].z,
                                 normalBuffer[objIndices[3]].r, normalBuffer[objIndices[3]].g, normalBuffer[objIndices[3]].b),
                                 &currentIndex, vertices, indices);
                addVertex(vertex(vertexBuffer[objIndices[4]].x, vertexBuffer[objIndices[4]].y, vertexBuffer[objIndices[4]].z,
                                 normalBuffer[objIndices[5]].r, normalBuffer[objIndices[5]].g, normalBuffer[objIndices[5]].b),
                                 &currentIndex, vertices, indices);
            }
        }
    }
}

void objMeshLoader::addVertex(const vertex& v, unsigned int* currentIndex, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) {
    auto position = std::find(vertices->begin(), vertices->end(), v);
    if (position != vertices->end()) {
        unsigned int index = position - vertices->begin();
        indices->push_back(index);
    } else {
        vertices->push_back(v);
        indices->push_back(*currentIndex);
        *currentIndex += 1;
    }
}
