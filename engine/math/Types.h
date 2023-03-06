#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

// Defined outside the chira namespace so it looks better in headers
using byte = std::uint8_t;

namespace glm {

using vec2b = vec<2, bool>;
using vec2i = vec<2, int>;
using vec2u = vec<2, unsigned int>;
using vec2f = vec<2, float>;
using vec2d = vec<2, double>;

using vec3b = vec<3, bool>;
using vec3i = vec<3, int>;
using vec3u = vec<3, unsigned int>;
using vec3f = vec<3, float>;
using vec3d = vec<3, double>;

using vec4b = vec<4, bool>;
using vec4i = vec<4, int>;
using vec4u = vec<4, unsigned int>;
using vec4f = vec<4, float>;
using vec4d = vec<4, double>;

constexpr const std::size_t VEC4F_SIZE = sizeof(vec4f);
constexpr const std::size_t MAT4_SIZE = sizeof(mat4);

} // namespace glm
