#ifndef BASICGAMEENGINE_VERTEX_H
#define BASICGAMEENGINE_VERTEX_H


#include "glad/gl.h"

struct vertex {
    [[nodiscard]] virtual int getSizeOf() const = 0;
};

struct vertex2 : public vertex {
    GLfloat u, v;
    vertex2(float a, float b);
    [[nodiscard]] int getSizeOf() const override;
};

struct vertex3 : public vertex {
    GLfloat x, y, z;
    vertex3(float a, float b, float c);
    [[nodiscard]] int getSizeOf() const override;
};


#endif //BASICGAMEENGINE_VERTEX_H
