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

void LightsUBO::update(DirectionalLightComponent* directionalLights[], PointLightComponent* pointLights[], SpotLightComponent* spotLights[], glm::vec3i numberOfLights) const {
    // todo: minimize the number of buffer writes
    unsigned int position = 0;
    int nullLights;

    nullLights = 0;
    for (int i = 0; i < DIRECTIONAL_LIGHT_MAX; i++) {
        auto* light = directionalLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->transform->getRotationEuler(), 0.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->ambient, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->diffuse, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->specular, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;
    }
    position += (glm::VEC4F_SIZE * 4) * nullLights;

    nullLights = 0;
    for (int i = 0; i < POINT_LIGHT_MAX; i++) {
        auto* light = pointLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->transform->getPosition(), 0.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->ambient, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->diffuse, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->specular, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->falloff, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;
    }
    position += (glm::VEC4F_SIZE * 5) * nullLights;

    nullLights = 0;
    for (int i = 0; i < SPOT_LIGHT_MAX; i++) {
        auto* light = spotLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->transform->getPosition(), 0.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->transform->getRotationEuler(), 0.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->diffuse, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->specular, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->falloff, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;

        Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{light->cutoff, 0.f, 1.f}), glm::VEC4F_SIZE);
        position += glm::VEC4F_SIZE;
    }
    position += (glm::VEC4F_SIZE * 6) * nullLights;

    Renderer::updateUniformBufferPart(this->handle, position, glm::value_ptr(glm::vec4{numberOfLights, 1.f}), glm::VEC4F_SIZE);
}
