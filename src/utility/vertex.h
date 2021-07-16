#ifndef BASICGAMEENGINE_VERTEX_H
#define BASICGAMEENGINE_VERTEX_H


#include "glad/gl.h"

struct position {
    GLfloat x, y, z;
    explicit position(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    friend bool operator==(const position& pos1, const position& pos2);
};

struct normal {
    GLfloat r, g, b;
    explicit normal(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0);
    friend bool operator==(const normal& pos1, const normal& pos2);
};

struct color {
    GLfloat r, g, b;
    explicit color(GLfloat r = 1, GLfloat g = 1, GLfloat b = 1);
    friend bool operator==(const color& pos1, const color& pos2);
};

struct uv {
    GLfloat u, v;
    explicit uv(GLfloat u = 0, GLfloat v = 0);
    friend bool operator==(const uv& uv1, const uv& uv2);
};

struct vertex {
    position position;
    normal normal;
    color color;
    uv uv;
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb, GLfloat u, GLfloat v);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
    vertex(GLfloat x, GLfloat y, GLfloat z);
    vertex();
    friend bool operator==(const vertex& v1, const vertex& v2);
};


#endif //BASICGAMEENGINE_VERTEX_H
