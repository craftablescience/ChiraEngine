#ifndef BASICGAMEENGINE_VERTEX_H
#define BASICGAMEENGINE_VERTEX_H


#include "glad/gl.h"

struct vertex {
    GLfloat x, y, z;
    vertex(float a, float b);
    vertex(float a, float b, float c);
    [[nodiscard]] int getSizeOf() const;
    [[nodiscard]] bool is2D() const;
    [[nodiscard]] bool is3D() const;
};


#endif //BASICGAMEENGINE_VERTEX_H
