#ifndef BASICGAMEENGINE_VERTEX_H
#define BASICGAMEENGINE_VERTEX_H


#include "glad/gl.h"

struct position {
    GLfloat x, y, z;
    position(GLfloat x, GLfloat y, GLfloat z);
};

struct normal {
    GLfloat r, g, b;
    normal(GLfloat r, GLfloat g, GLfloat b);
};

struct uv {
    GLfloat u, v;
    uv(GLfloat u, GLfloat v);
};

struct vertex {
    position position;
    normal normal;
    uv uv;
    vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v);
};


#endif //BASICGAMEENGINE_VERTEX_H
