#include "primitiveMeshLoader.h"

#include <utility/logger.h>
#include <i18n/translationManager.h>

using namespace chira;

void primitiveMeshLoader::loadMesh(const std::string& identifier, std::vector<vertex>* vertices, std::vector<unsigned int>* indices) {
    if (identifier == "plane") {
        vertices->assign({
            vertex( 1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
            vertex( 1.0f,-1.0f, 0.0f, 1.0f, 0.0f),
            vertex(-1.0f,-1.0f, 0.0f, 0.0f, 0.0f),
            vertex(-1.0f, 1.0f, 0.0f, 0.0f, 1.0f)
        });
        indices->assign({
            0, 1, 3,
            1, 2, 3
        });
    } else if (identifier == "cube") {
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
        logger::log(ERR, "PrimitiveMeshLoader", TR("error.primitive_loader.invalid_identifier"));
    }
}
