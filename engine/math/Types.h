#pragma once

#include <Jolt/Jolt.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>

// Defined outside the chira namespace so it looks better in headers
using byte = std::uint8_t;

JPH_NAMESPACE_BEGIN

inline glm::vec3 toGLM(RVec3 vec3) {
    return {static_cast<float>(vec3.GetX()), static_cast<float>(vec3.GetY()), static_cast<float>(vec3.GetZ())};
}

inline glm::vec4 toGLM(Vec4 vec4) {
    return {vec4.GetX(), vec4.GetY(), vec4.GetZ(), vec4.GetW()};
}

inline glm::mat4 toGLM(RMat44 mat44) {
    glm::mat4 out;
    auto mat4 = mat44.ToMat44();
    out[0] = toGLM(mat4.GetColumn4(0));
    out[1] = toGLM(mat4.GetColumn4(1));
    out[2] = toGLM(mat4.GetColumn4(2));
    out[3] = toGLM(mat4.GetColumn4(3));
    return out;
}

JPH_NAMESPACE_END

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
