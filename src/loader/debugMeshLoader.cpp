#include "debugMeshLoader.h"

void debugMeshLoader::loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) {
    vertices->assign({
        vertex( 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        vertex( 0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        vertex(-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        vertex(-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f)
    });
    indices->assign({
        0, 1, 3,
        1, 2, 3
    });
}
