#pragma once

#include <cstddef>
#include <string>
#include <entity/light/LightManager.h>
#include <math/Types.h>
#include <render/backend/Renderer.h>

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
struct PerspectiveViewUBO final : public UniformBufferObject<(3 * glm::MAT4_SIZE) + (2 * glm::VEC4_SIZE)> {
    static PerspectiveViewUBO& get();
    void update(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos, glm::vec3 viewLookDir) const;
private:
    PerspectiveViewUBO();
};

constexpr const std::size_t DIRECTIONAL_LIGHT_DATA_SIZE = glm::VEC4_SIZE  + sizeof(DirectionalLightData);
constexpr const std::size_t POINT_LIGHT_DATA_SIZE       = glm::VEC4_SIZE  + sizeof(PointLightData);
constexpr const std::size_t SPOT_LIGHT_DATA_SIZE   = (2 * glm::VEC4_SIZE) + sizeof(SpotLightData);

/// Stores lights
struct LightsUBO final : public UniformBufferObject<(DIRECTIONAL_LIGHT_DATA_SIZE * DIRECTIONAL_LIGHT_COUNT) +
                                                    (POINT_LIGHT_DATA_SIZE       * POINT_LIGHT_COUNT) +
                                                    (SPOT_LIGHT_DATA_SIZE        * SPOT_LIGHT_COUNT) +
                                                    glm::VEC4_SIZE> {
    static LightsUBO& get();
    void update(DirectionalLight* directionalLights[], PointLight* pointLights[], SpotLight* spotLights[], glm::vec3 numberOfLights) const;
private:
    LightsUBO();
};

} // namespace chira
