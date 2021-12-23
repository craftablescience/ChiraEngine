#include "vertex.h"

using namespace chira;

position::position(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

normal::normal(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

color::color(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

uv::uv(GLfloat u, GLfloat v) {
    this->u = u;
    this->v = v;
}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb, GLfloat u, GLfloat v)
    : pos(x, y, z), norm(r, g, b), col(rr, gg, bb), uvMap(u, v) {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb)
    : pos(x, y, z), norm(r, g, b), col(rr, gg, bb), uvMap() {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v)
    : pos(x, y, z), norm(r, g, b), col(), uvMap(u, v) {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
    : pos(x, y, z), norm(r, g, b), col(), uvMap() {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
    : pos(x, y, z), norm(), col(), uvMap(u, v) {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z)
    : pos(x, y, z), norm(), col(), uvMap() {}

vertex::vertex(const struct position& newPos, const struct normal& newNorm, const struct color& newCol, const struct uv& newTex) {
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

vertex::vertex() : pos(), norm(), col(), uvMap() {}
