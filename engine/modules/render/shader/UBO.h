#pragma once

#include <cstddef>
#include <string>

#include <core/math/Types.h>
#include <entity/component/LightComponents.h>
#include <render/backend/RenderBackend.h>

namespace chira {

template<std::ptrdiff_t Size>
class UniformBufferObject {
protected:
    explicit UniformBufferObject(std::string name_)
        : name(std::move(name_))
        , handle(Renderer::createUniformBuffer(Size)) {}
public:
    virtual ~UniformBufferObject() {
        Renderer::destroyUniformBuffer(this->handle);
    }
    inline void bindToShader(Renderer::ShaderHandle shaderHandle) {
        Renderer::bindUniformBufferToShader(shaderHandle, this->handle, this->name);
    }
protected:
    std::string name;
    Renderer::UniformBufferHandle handle;

    static constexpr std::ptrdiff_t size = Size;
};

/// Stores two mat4 values, named PV
struct PerspectiveViewUBO final : public UniformBufferObject<(3 * glm::MAT4_SIZE) + (2 * glm::VEC4F_SIZE)> {
    static PerspectiveViewUBO& get();
    void update(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos, glm::vec3 viewLookDir) const;
private:
    PerspectiveViewUBO();
};

/// Stores lights
struct LightsUBO final : public UniformBufferObject<((4 * glm::VEC4F_SIZE) * DIRECTIONAL_LIGHT_MAX) +
                                                    ((5 * glm::VEC4F_SIZE) * POINT_LIGHT_MAX) +
                                                    ((6 * glm::VEC4F_SIZE) * SPOT_LIGHT_MAX) +
                                                    glm::VEC4F_SIZE> {
    static LightsUBO& get();
    void update(DirectionalLightComponent* directionalLights[], PointLightComponent* pointLights[], SpotLightComponent* spotLights[], glm::vec3i numberOfLights) const;
private:
    LightsUBO();
};

} // namespace chira
