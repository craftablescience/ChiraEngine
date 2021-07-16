#include "vertex.h"

position::position(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

bool operator==(const position &pos1, const position &pos2) {
    return pos1.x == pos2.x && pos1.y == pos2.y && pos1.z == pos2.z;
}

normal::normal(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

bool operator==(const normal &norm1, const normal &norm2) {
    return norm1.r == norm2.r && norm1.g == norm2.g && norm1.b == norm2.b;
}

color::color(GLfloat r, GLfloat g, GLfloat b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

bool operator==(const color &color1, const color &color2) {
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
}

uv::uv(GLfloat u, GLfloat v) {
    this->u = u;
    this->v = v;
}

bool operator==(const uv &uv1, const uv &uv2) {
    return uv1.u == uv2.u && uv1.v == uv2.v;
}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb, GLfloat u, GLfloat v)
    : position(x, y, z), normal(r, g, b), color(rr, gg, bb), uv(u, v) {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb)
    : position(x, y, z), normal(r, g, b), color(rr, gg, bb), uv() {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v)
    : position(x, y, z), normal(r, g, b), color(), uv(u, v) {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
    : position(x, y, z), normal(r, g, b), color(), uv() {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
    : position(x, y, z), normal(), color(), uv(u, v) {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z)
    : position(x, y, z), normal(), color(), uv() {}

vertex::vertex() : position(), normal(), color(), uv() {}

bool operator==(const vertex &v1, const vertex &v2) {
    return v1.position == v2.position && v1.normal == v2.normal && v1.color == v2.color && v1.uv == v2.uv;
}
