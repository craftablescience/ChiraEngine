#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>

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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec2b, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec2i, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec2u, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec2f, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec2d, x, y)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec3b, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec3i, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec3u, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec3f, x, y, z)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec3d, x, y, z)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec4b, x, y, z, w)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec4i, x, y, z, w)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec4u, x, y, z, w)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec4f, x, y, z, w)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vec4d, x, y, z, w)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(quat, x, y, z, w)

} // namespace glm
