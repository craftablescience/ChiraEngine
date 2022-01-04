#include "vertex.h"

using namespace chira;

Position::Position(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Normal::Normal(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

UV::UV(GLfloat u, GLfloat v) {
    this->u = u;
    this->v = v;
}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb, GLfloat u, GLfloat v)
    : pos(x, y, z), norm(r, g, b), col(rr, gg, bb), uvMap(u, v) {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb)
    : pos(x, y, z), norm(r, g, b), col(rr, gg, bb), uvMap() {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v)
    : pos(x, y, z), norm(r, g, b), col(), uvMap(u, v) {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
    : pos(x, y, z), norm(r, g, b), col(), uvMap() {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
    : pos(x, y, z), norm(), col(), uvMap(u, v) {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z)
    : pos(x, y, z), norm(), col(), uvMap() {}

Vertex::Vertex(const struct Position& newPos, const struct Normal& newNorm, const struct Color& newCol, const struct UV& newTex) {
    this->pos.x = newPos.x;
    this->pos.y = newPos.y;
    this->pos.z = newPos.z;
    this->norm.r = newNorm.r;
    this->norm.g = newNorm.g;
    this->norm.b = newNorm.b;
    this->col.r = newCol.r;
    this->col.g = newCol.g;
    this->col.b = newCol.b;
    this->uvMap.u = newTex.u;
    this->uvMap.v = newTex.v;
}

Vertex::Vertex() : pos(), norm(), col(), uvMap() {}
