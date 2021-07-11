#include "debugMeshLoader.h"

#if DEBUG
#include <stdexcept>
#endif

void debugMeshLoader::loadMesh(const std::string& filepath, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) {
    if (filepath == "plane") {
        vertices->assign({
            vertex( 0.5f, 0.5f, 0.0f, 1.0f, 1.0f),
            vertex( 0.5f,-0.5f, 0.0f, 1.0f, 0.0f),
            vertex(-0.5f,-0.5f, 0.0f, 0.0f, 0.0f),
            vertex(-0.5f, 0.5f, 0.0f, 0.0f, 1.0f)
        });
        indices->assign({
            0, 1, 3,
            1, 2, 3
        });
    } else if (filepath == "cube") {
        vertices->assign({
            vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f),
            vertex(0.5f, -0.5f, -0.5f,  1.0f, 0.0f),
            vertex(0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
            vertex(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f),
            vertex(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
            vertex(0.5f, -0.5f,  0.5f,  1.0f, 0.0f),
            vertex(0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
            vertex(-0.5f,  0.5f,  0.5f,  0.0f, 1.0f),

            vertex(-0.5f,  0.5f, -0.5f,  0.0f, 0.0f),
            vertex(-0.5f, -0.5f, -0.5f,  1.0f, 0.0f),
            vertex(-0.5f, -0.5f,  0.5f,  1.0f, 1.0f),
            vertex(-0.5f,  0.5f,  0.5f,  0.0f, 1.0f),
            vertex(0.5f, -0.5f, -0.5f,  0.0f, 0.0f),
            vertex(0.5f,  0.5f, -0.5f,  1.0f, 0.0f),
            vertex(0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
            vertex(0.5f, -0.5f,  0.5f,  0.0f, 1.0f),

            vertex(-0.5f, -0.5f, -0.5f,  0.0f, 0.0f),
            vertex(0.5f, -0.5f, -0.5f,  1.0f, 0.0f),
            vertex(0.5f, -0.5f,  0.5f,  1.0f, 1.0f),
            vertex(-0.5f, -0.5f,  0.5f,  0.0f, 1.0f),
            vertex(0.5f,  0.5f, -0.5f,   0.0f, 0.0f),
            vertex(-0.5f,  0.5f, -0.5f,  1.0f, 0.0f),
            vertex(-0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
            vertex(0.5f,  0.5f,  0.5f,   0.0f, 1.0f)
        });
        indices->assign({
             0,  3,  2,
             2,  1,  0,
             4,  5,  6,
             6,  7,  4,
            11,  8,  9,
             9, 10, 11,
            12, 13, 14,
            14, 15, 12,
            16, 17, 18,
            18, 19, 16,
            20, 21, 22,
            22, 23, 20
        });
    } else {
#if DEBUG
        throw std::runtime_error("File path for the debug mesh loader must be a valid name (example: \"cube\")");
#endif
    }
}
