#include "primitiveMeshLoader.h"

#include <utility/logger.h>
#include <i18n/translationManager.h>
#include <utility/assertions.h>

using namespace chira;

void PrimitiveMeshLoader::loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    if (identifier == "plane") {
        vertices.assign({
            Vertex( 1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
            Vertex( 1.0f,-1.0f, 0.0f, 1.0f, 0.0f),
            Vertex(-1.0f,-1.0f, 0.0f, 0.0f, 0.0f),
            Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 1.0f)
        });
        indices.assign({
            0, 1, 3,
            1, 2, 3
        });
    } else if (identifier == "cube") {
        vertices.assign({
            Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f),
            Vertex(0.5f, -0.5f, -0.5f,  1.0f, 0.0f),
            Vertex(0.5f,  0.5f, -0.5f,  1.0f, 1.0f),
            Vertex(-0.5f,  0.5f, -0.5f,  0.0f, 1.0f),
            Vertex(-0.5f, -0.5f,  0.5f,  0.0f, 0.0f),
            Vertex(0.5f, -0.5f,  0.5f,  1.0f, 0.0f),
            Vertex(0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
            Vertex(-0.5f,  0.5f,  0.5f,  0.0f, 1.0f),

            Vertex(-0.5f,  0.5f, -0.5f,  0.0f, 0.0f),
            Vertex(-0.5f, -0.5f, -0.5f,  1.0f, 0.0f),
            Vertex(-0.5f, -0.5f,  0.5f,  1.0f, 1.0f),
            Vertex(-0.5f,  0.5f,  0.5f,  0.0f, 1.0f),
            Vertex(0.5f, -0.5f, -0.5f,  0.0f, 0.0f),
            Vertex(0.5f,  0.5f, -0.5f,  1.0f, 0.0f),
            Vertex(0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
            Vertex(0.5f, -0.5f,  0.5f,  0.0f, 1.0f),

            Vertex(-0.5f, -0.5f, -0.5f,  0.0f, 0.0f),
            Vertex(0.5f, -0.5f, -0.5f,  1.0f, 0.0f),
            Vertex(0.5f, -0.5f,  0.5f,  1.0f, 1.0f),
            Vertex(-0.5f, -0.5f,  0.5f,  0.0f, 1.0f),
            Vertex(0.5f,  0.5f, -0.5f,   0.0f, 0.0f),
            Vertex(-0.5f,  0.5f, -0.5f,  1.0f, 0.0f),
            Vertex(-0.5f,  0.5f,  0.5f,  1.0f, 1.0f),
            Vertex(0.5f,  0.5f,  0.5f,   0.0f, 1.0f)
        });
        indices.assign({
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
        Logger::log(LogType::ERROR, "PrimitiveMeshLoader", TR("error.primitive_loader.invalid_identifier"));
    }
}

std::vector<byte> PrimitiveMeshLoader::createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    // todo(i18n)
    chira_assert(false, "It should be impossible to ever call this method!");
    return {};
}
