#pragma once

#include <glm/glm.hpp>
#include "Color.h"

namespace chira {

struct Vertex {
    glm::vec3 position;
    ColorRGB normal;
    ColorRGB color;
    ColorRG uv;
    Vertex();
    explicit Vertex(glm::vec3 pos);
    Vertex(glm::vec3 pos, ColorRGB norm);
    Vertex(glm::vec3 pos, ColorRGB norm, ColorRGB col);
    Vertex(glm::vec3 pos, ColorRGB norm, ColorRGB col, ColorRG tex);
    Vertex(glm::vec3 pos, ColorRGB norm, ColorRG tex);
    Vertex(glm::vec3 pos, ColorRG tex);
    bool operator==(const Vertex& other) const {
        return this->position == other.position &&
               this->normal == other.normal &&
               this->color == other.color &&
               this->uv == other.uv;
    }
    bool operator!=(const Vertex& other) const {
        return !this->operator==(other);
    }
};

} // namespace chira
