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
    : position(x, y, z), normal(r, g, b), color(rr, gg, bb), uv(u, v) {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb)
    : position(x, y, z), normal(r, g, b), color(rr, gg, bb), uv() {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v)
    : position(x, y, z), normal(r, g, b), color(), uv(u, v) {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
    : position(x, y, z), normal(r, g, b), color(), uv() {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
    : position(x, y, z), normal(), color(), uv(u, v) {}

Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z)
    : position(x, y, z), normal(), color(), uv() {}

Vertex::Vertex(const struct Position& newPos, const struct Normal& newNorm, const struct Color& newCol, const struct UV& newTex) {
    this->position.x = newPos.x;
    this->position.y = newPos.y;
    this->position.z = newPos.z;
    this->normal.r = newNorm.r;
    this->normal.g = newNorm.g;
    this->normal.b = newNorm.b;
    this->color.r = newCol.r;
    this->color.g = newCol.g;
    this->color.b = newCol.b;
    this->uv.u = newTex.u;
    this->uv.v = newTex.v;
}

Vertex::Vertex() : position(), normal(), color(), uv() {}
