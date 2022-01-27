#include "vertex.h"

using namespace chira;

Vertex::Vertex()
    : position()
    , normal()
    , color(1, 1, 1)
    , uv() {}

Vertex::Vertex(glm::vec3 pos)
    : position(pos)
    , normal()
    , color(1, 1, 1)
    , uv() {}

Vertex::Vertex(glm::vec3 pos, ColorRGB norm)
    : position(pos)
    , normal(norm)
    , color(1, 1, 1)
    , uv() {}

Vertex::Vertex(glm::vec3 pos, ColorRGB norm, ColorRGB col)
    : position(pos)
    , normal(norm)
    , color(col)
    , uv() {}

Vertex::Vertex(glm::vec3 pos, ColorRGB norm, ColorRGB col, ColorRG tex)
    : position(pos)
    , normal(norm)
    , color(col)
    , uv(tex) {}

Vertex::Vertex(glm::vec3 pos, ColorRGB norm, ColorRG tex)
    : position(pos)
    , normal(norm)
    , color(1, 1, 1)
    , uv(tex) {}

Vertex::Vertex(glm::vec3 pos, ColorRG tex)
    : position(pos)
    , normal()
    , color(1, 1, 1)
    , uv(tex) {}
