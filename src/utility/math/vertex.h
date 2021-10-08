#pragma once

#include "glad/gl.h"

namespace chira {
    struct position {
        GLfloat x, y, z;
        explicit position(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
        bool operator==(const position& other) const {
            return this->x == other.x && this->y == other.y && this->z == other.z;
        }
    };

    struct normal {
        GLfloat r, g, b;
        explicit normal(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0);
        bool operator==(const normal& other) const {
            return this->r == other.r && this->g == other.g && this->b == other.b;
        }
    };

    struct color {
        GLfloat r, g, b;
        explicit color(GLfloat r = 1, GLfloat g = 1, GLfloat b = 1);
        bool operator==(const color& other) const {
            return this->r == other.r && this->g == other.g && this->b == other.b;
        }
    };

    struct uv {
        GLfloat u, v;
        explicit uv(GLfloat u = 0, GLfloat v = 0);
        bool operator==(const uv& other) const {
            return this->u == other.u && this->v == other.v;
        }
    };

    struct vertex {
        position pos;
        normal norm;
        color col;
        uv uvMap;
        vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb, GLfloat u, GLfloat v);
        vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb);
        vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v);
        vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
        vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
        vertex(GLfloat x, GLfloat y, GLfloat z);
        vertex(const struct position& pos, const struct normal& norm, const struct color& col, const struct uv& tex);
        vertex();
        bool operator==(const vertex& other) const {
            return this->pos == other.pos && this->norm == other.norm && this->col == other.col && this->uvMap == other.uvMap;
        }
    };
}
