#include "vertex.h"

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

uv::uv(GLfloat u, GLfloat v) {
    this->u = u;
    this->v = v;
}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v)
    : position(x, y, z), normal(r, g, b), uv(u, v) {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
    : position(x, y, z), normal(r, g, b), uv() {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
    : position(x, y, z), normal(), uv(u, v) {}

vertex::vertex(GLfloat x, GLfloat y, GLfloat z)
    : position(x, y, z), normal(), uv() {}

vertex::vertex() : position(), normal(), uv() {}
