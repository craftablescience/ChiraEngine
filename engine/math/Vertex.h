#pragma once

#include <glm/glm.hpp>

#include "Color.h"

namespace chira {

struct Vertex {
    glm::vec3 position;
    ColorRGB normal;
    ColorRGB color;
    ColorRG uv;

    Vertex()
            : position()
            , normal()
            , color(1, 1, 1)
            , uv() {}
    explicit Vertex(glm::vec3 pos)
            : position(pos)
            , normal()
            , color(1, 1, 1)
            , uv() {}
    Vertex(glm::vec3 pos, ColorRGB norm)
            : position(pos)
            , normal(norm)
            , color(1, 1, 1)
            , uv() {}
    Vertex(glm::vec3 pos, ColorRGB norm, ColorRGB col)
            : position(pos)
            , normal(norm)
            , color(col)
            , uv() {}
    Vertex(glm::vec3 pos, ColorRGB norm, ColorRGB col, ColorRG tex)
            : position(pos)
            , normal(norm)
            , color(col)
            , uv(tex) {}
    Vertex(glm::vec3 pos, ColorRGB norm, ColorRG tex)
            : position(pos)
            , normal(norm)
            , color(1, 1, 1)
            , uv(tex) {}
    Vertex(glm::vec3 pos, ColorRG tex)
            : position(pos)
            , normal()
            , color(1, 1, 1)
            , uv(tex) {}

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

using Index = unsigned int;

} // namespace chira
