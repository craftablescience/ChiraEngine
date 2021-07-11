#ifndef BASICGAMEENGINE_VERTEX_H
#define BASICGAMEENGINE_VERTEX_H


#include "glad/gl.h"

struct position {
    GLfloat x, y, z;
    explicit position(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
};

struct normal {
    GLfloat r, g, b;
    explicit normal(GLfloat r = 1, GLfloat g = 1, GLfloat b = 1);
};

struct uv {
    GLfloat u, v;
    explicit uv(GLfloat u = 0, GLfloat v = 0);
};

struct vertex {
    position position;
    normal normal;
    uv uv;
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
    vertex(GLfloat x, GLfloat y, GLfloat z);
    vertex();
};


#endif //BASICGAMEENGINE_VERTEX_H
