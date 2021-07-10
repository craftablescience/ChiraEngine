#include "vertex.h"

vertex2::vertex2(float a, float b) {
    this->u = a;
    this->v = b;
}

int vertex2::getSizeOf() const {
    return (2 * sizeof(float));
}

vertex3::vertex3(float a, float b, float c) {
    this->x = a;
    this->y = b;
    this->z = c;
}

int vertex3::getSizeOf() const {
    return (3 * sizeof(float));
}
