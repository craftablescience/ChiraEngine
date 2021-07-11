#include "vertex.h"
#include <cmath>

vertex::vertex(float a, float b) {
    this->x = a;
    this->y = b;
    this->z = NAN;
}

vertex::vertex(float a, float b, float c) {
    this->x = a;
    this->y = b;
    this->z = c;
}

int vertex::getSizeOf() const {
    if (std::isnan(this->z)) {
        return (2 * sizeof(float));
    } else {
        return (3 * sizeof(float));
    }
}

bool vertex::is2D() const {
    return std::isnan(this->z);
}

bool vertex::is3D() const {
    return !std::isnan(this->z);
}
