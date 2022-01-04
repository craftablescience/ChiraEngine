#pragma once

#include <glad/gl.h>

namespace chira {
    struct Position {
        GLfloat x, y, z;
        explicit Position(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
        bool operator==(const Position& other) const {
            return this->x == other.x && this->y == other.y && this->z == other.z;
        }
        bool operator!=(const Position& other) const {
            return !operator==(other);
        }
    };

    struct Normal {
        GLfloat r, g, b;
        explicit Normal(GLfloat r = 0, GLfloat g = 0, GLfloat b = 0);
        bool operator==(const Normal& other) const {
            return this->r == other.r && this->g == other.g && this->b == other.b;
        }
        bool operator!=(const Normal& other) const {
            return !operator==(other);
        }
    };

    struct Color {
        GLfloat r, g, b;
        explicit Color(GLfloat r = 1, GLfloat g = 1, GLfloat b = 1);
        bool operator==(const Color& other) const {
            return this->r == other.r && this->g == other.g && this->b == other.b;
        }
        bool operator!=(const Color& other) const {
            return !operator==(other);
        }
    };

    struct UV {
        GLfloat u, v;
        explicit UV(GLfloat u = 0, GLfloat v = 0);
        bool operator==(const UV& other) const {
            return this->u == other.u && this->v == other.v;
        }
        bool operator!=(const UV& other) const {
            return !operator==(other);
        }
    };

    struct Vertex {
        Position pos;
        Normal norm;
        Color col;
        UV uvMap;
        Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb, GLfloat u, GLfloat v);
        Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat rr, GLfloat gg, GLfloat bb);
        Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat u, GLfloat v);
        Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
        Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);
        Vertex(GLfloat x, GLfloat y, GLfloat z);
        Vertex(const struct Position& pos, const struct Normal& norm, const struct Color& col, const struct UV& tex);
        Vertex();
        bool operator==(const Vertex& other) const {
            return this->pos == other.pos && this->norm == other.norm && this->col == other.col && this->uvMap == other.uvMap;
        }
        bool operator!=(const Vertex& other) const {
            return !operator==(other);
        }
    };
}
