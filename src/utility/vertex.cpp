#include "vertex.h"

position::position(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

bool operator==(const position& pos1, const position& pos2) {
    return pos1.x == pos2.x && pos1.y == pos2.y && pos1.z == pos2.z;
}

normal::normal(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

bool operator==(const normal& norm1, const normal& norm2) {
    return norm1.r == norm2.r && norm1.g == norm2.g && norm1.b == norm2.b;
}

color::color(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

bool operator==(const color& col1, const color& col2) {
    return col1.r == col2.r && col1.g == col2.g && col1.b == col2.b;
}

uv::uv(GLfloat u, GLfloat v) {
    this->u = u;
    this->v = v;
}

bool operator==(const uv& uv1, const uv& uv2) {
    return uv1.u == uv2.u && uv1.v == uv2.v;
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

bool operator==(const vertex& v1, const vertex& v2) {
    return v1.pos == v2.pos && v1.norm == v2.norm && v1.col == v2.col && v1.uvMap == v2.uvMap;
}
