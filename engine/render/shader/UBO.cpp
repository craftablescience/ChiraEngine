#include "UBO.h"

#include <glm/gtc/type_ptr.hpp>

using namespace chira;

PerspectiveViewUBO::PerspectiveViewUBO() : UniformBufferObject("PV") {}

PerspectiveViewUBO& PerspectiveViewUBO::get() {
    static PerspectiveViewUBO singleton;
    return singleton;
}

void PerspectiveViewUBO::update(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos, glm::vec3 viewLookDir) const {
    // todo: minimize the number of buffer writes
    Renderer::updateUniformBufferPart(this->handle, 0 * glm::MAT4_SIZE, &proj, glm::MAT4_SIZE);
    Renderer::updateUniformBufferPart(this->handle, 1 * glm::MAT4_SIZE, &view, glm::MAT4_SIZE);
    Renderer::updateUniformBufferPart(this->handle, 2 * glm::MAT4_SIZE, glm::value_ptr(proj * view), glm::MAT4_SIZE);
    Renderer::updateUniformBufferPart(this->handle, 3 * glm::MAT4_SIZE, glm::value_ptr(glm::vec4{viewPos, 1.0}), glm::VEC4F_SIZE);
    Renderer::updateUniformBufferPart(this->handle, (3 * glm::MAT4_SIZE) + glm::VEC4F_SIZE, glm::value_ptr(glm::vec4{viewLookDir, 1.0}), glm::VEC4F_SIZE);
}

LightsUBO::LightsUBO() : UniformBufferObject("LIGHTS") {}

LightsUBO& LightsUBO::get() {
    static LightsUBO singleton;
    return singleton;
}

void LightsUBO::update(DirectionalLight* directionalLights[], PointLight* pointLights[], SpotLight* spotLights[], glm::vec3 numberOfLights) const {
    // todo: minimize the number of buffer writes
    unsigned int position = 0;
    int nullLights;

    nullLights = 0;
    for (int i = 0; i < DIRECTIONAL_LIGHT_COUNT; i++) {
        auto* light = directionalLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        glm::vec4 dir{glm::eulerAngles(light->getRotation()), 0.f};
        Renderer::updateUniformBufferPart(this->handle, position, &dir, glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, light->getLightData(), sizeof(DirectionalLightData));
        position += sizeof(DirectionalLightData);
    }
    position += DIRECTIONAL_LIGHT_DATA_SIZE * nullLights;

    nullLights = 0;
    for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
        auto* light = pointLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        glm::vec4 pos{light->getGlobalPosition(), 0.f};
        Renderer::updateUniformBufferPart(this->handle, position, &pos, glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, light->getLightData(), sizeof(PointLightData));
        position += sizeof(PointLightData);
    }
    position += POINT_LIGHT_DATA_SIZE * nullLights;

    nullLights = 0;
    for (int i = 0; i < SPOT_LIGHT_COUNT; i++) {
        auto* light = spotLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        glm::vec4 pos{light->getGlobalPosition(), 0.f};
        Renderer::updateUniformBufferPart(this->handle, position, &pos, glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        glm::vec4 dir{glm::eulerAngles(light->getRotation()), 0.f};
        Renderer::updateUniformBufferPart(this->handle, position, &dir, glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, light->getLightData(), sizeof(SpotLightData));
        position += sizeof(SpotLightData);
    }
    position += SPOT_LIGHT_DATA_SIZE * nullLights;

    glm::vec4 counts{numberOfLights, 1.f};
    Renderer::updateUniformBufferPart(this->handle, position, &counts, glm::VEC4F_SIZE);
}
