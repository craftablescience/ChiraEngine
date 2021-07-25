#include <fstream>
#include <sstream>
#include <algorithm>
#include "objMeshLoader.h"
#include "../core/engine.h"

void objMeshLoader::loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) {
    std::vector<position> vertexBuffer;
    std::vector<uv> uvBuffer;
    std::vector<normal> normalBuffer;

    std::ifstream input(filepath);
    std::string line;
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
            // Check to make sure it is triangulated
            std::istringstream ss(line.substr(2));
            std::string word;
            unsigned int count = 0;
            while (ss >> word) {
                count++;
            }
            if (count > 3) {
                engine::logError("OBJ", "OBJ file " + filepath + " is not triangulated");
            }
            // Figure out if UVs are included
            bool includeUVs = true;
            std::string data = line.substr(2);
            std::replace(data.begin(), data.end(), '/', ' ');
            std::istringstream uss(data);
            unsigned int i;
            unsigned int c = 0;
            while (uss >> i) {
                c++;
            }
            if (c < 9) {
                includeUVs = false;
            }
            // Parse
            std::istringstream iss(data);
            int p0, p1, p2, t0 = 0, t1 = 0, t2 = 0, n0, n1, n2;
            if (includeUVs) {
                iss >> p0 >> t0 >> n0 >> p1 >> t1 >> n1 >> p2 >> t2 >> n2;
                p0 -= 1;
                t0 -= 1;
                n0 -= 1;
                p1 -= 1;
                t1 -= 1;
                n1 -= 1;
                p2 -= 1;
                t2 -= 1;
                n2 -= 1;
                addVertex(vertex(vertexBuffer[p0].x, vertexBuffer[p0].y, vertexBuffer[p0].z,
                                    normalBuffer[n0].r, normalBuffer[n0].g, normalBuffer[n0].b,
                                    uvBuffer[t0].u, uvBuffer[t0].v), vertices, indices);
                addVertex(vertex(vertexBuffer[p1].x, vertexBuffer[p1].y, vertexBuffer[p1].z,
                                    normalBuffer[n1].r, normalBuffer[n1].g, normalBuffer[n1].b,
                                    uvBuffer[t1].u, uvBuffer[t1].v), vertices, indices);
                addVertex(vertex(vertexBuffer[p2].x, vertexBuffer[p2].y, vertexBuffer[p2].z,
                                    normalBuffer[n2].r, normalBuffer[n2].g, normalBuffer[n2].b,
                                    uvBuffer[t2].u, uvBuffer[t2].v), vertices, indices);
            } else {
                iss >> p0 >> n0 >> p1 >> n1 >> p2 >> n2;
                p0 -= 1;
                n0 -= 1;
                p1 -= 1;
                n1 -= 1;
                p2 -= 1;
                n2 -= 1;
                addVertex(vertex(vertexBuffer[p0].x, vertexBuffer[p0].y, vertexBuffer[p0].z,
                                    normalBuffer[n0].r, normalBuffer[n0].g, normalBuffer[n0].b), vertices, indices);
                addVertex(vertex(vertexBuffer[p1].x, vertexBuffer[p1].y, vertexBuffer[p1].z,
                                    normalBuffer[n1].r, normalBuffer[n1].g, normalBuffer[n1].b), vertices, indices);
                addVertex(vertex(vertexBuffer[p2].x, vertexBuffer[p2].y, vertexBuffer[p2].z,
                                    normalBuffer[n2].r, normalBuffer[n2].g, normalBuffer[n2].b), vertices, indices);
            }
        }
    }
}

void objMeshLoader::addVertex(const vertex& v, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) {
    // todo: properly use EBOs
    vertices->push_back(v);
    if (!indices->empty()) {
        indices->push_back(indices->at(indices->size() - 1) + 1);
    } else {
        indices->push_back(0);
    }
    /*
    auto position = std::find(vertices->begin(), vertices->end(), v);
    if (position != vertices->end()) {
        unsigned int index = position - vertices->begin();
        indices->push_back(index);
    } else {
        vertices->push_back(v);
        if (!indices->empty()) {
            indices->push_back(indices->at(indices->size() - 1) + 1);
        } else {
            indices->push_back(0);
        }
    }
    */
}
