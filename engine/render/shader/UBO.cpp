#include "UBO.h"

#include <glm/gtc/type_ptr.hpp>
#include <entity/light/LightManager.h>
#include "Shader.h"

using namespace chira;

UniformBufferObject::UniformBufferObject(std::string name_, unsigned int size) : HandleObject<unsigned int>(0), name(std::move(name_)) {
    this->bindingPoint = UniformBufferObject::uboBindingPoint++;
    glGenBuffers(1, &this->handle);
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, this->bindingPoint);
    glBindBufferRange(GL_UNIFORM_BUFFER, this->bindingPoint, this->handle, 0, size);
}

unsigned int UniformBufferObject::getBindingPoint() const {
    return this->bindingPoint;
}

void UniformBufferObject::bindToShader(Shader* shader_) const {
    glUniformBlockBinding(shader_->getHandle(), glGetUniformBlockIndex(shader_->getHandle(), this->name.c_str()), this->bindingPoint);
}

void UniformBufferObject::update(const byte buffer[], GLsizeiptr length) const {
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferData(GL_UNIFORM_BUFFER, length, buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

static constexpr const std::size_t MAT4_SIZE = sizeof(glm::mat4);
static constexpr const std::size_t VEC4_SIZE = sizeof(glm::vec4);

PerspectiveViewUBO::PerspectiveViewUBO() : UniformBufferObject("PV", (3 * MAT4_SIZE) + (2 * VEC4_SIZE)) {}

PerspectiveViewUBO* PerspectiveViewUBO::get() {
    if (!PerspectiveViewUBO::singleton)
        PerspectiveViewUBO::singleton.reset(new PerspectiveViewUBO{});
    return PerspectiveViewUBO::singleton.get();
}

void PerspectiveViewUBO::update(glm::mat4 proj, glm::mat4 view, glm::vec3 viewPos, glm::vec3 viewLookDir) const {
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * MAT4_SIZE, MAT4_SIZE, &proj);
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * MAT4_SIZE, MAT4_SIZE, &view);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * MAT4_SIZE, MAT4_SIZE, glm::value_ptr(proj * view));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * MAT4_SIZE, VEC4_SIZE, glm::value_ptr(glm::vec4{viewPos, 1.0}));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * MAT4_SIZE + VEC4_SIZE, VEC4_SIZE, glm::value_ptr(glm::vec4{viewLookDir, 1.0}));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

LightsUBO::LightsUBO() : UniformBufferObject("LIGHTS", (4 * VEC4_SIZE * DIRECTIONAL_LIGHT_COUNT) +
                                                       (5 * VEC4_SIZE * POINT_LIGHT_COUNT) +
                                                       (6 * VEC4_SIZE * SPOT_LIGHT_COUNT) +
                                                       VEC4_SIZE) {}

LightsUBO* LightsUBO::get() {
    if (!LightsUBO::singleton)
        LightsUBO::singleton.reset(new LightsUBO{});
    return LightsUBO::singleton.get();
}

void LightsUBO::updateLights(DirectionalLight* directionalLights[], PointLight* pointLights[], SpotLight* spotLights[], glm::vec3 numberOfLights) const {
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    unsigned int position = 0;
    unsigned short nullLights = 0;

    for (int i = 0; i < DIRECTIONAL_LIGHT_COUNT; i++) {
        auto* light = directionalLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        glm::vec4 dir =  {glm::eulerAngles(light->getRotation()), 0.f};
        glm::vec4 amb =  {light->getLightData().ambient,          1.f};
        glm::vec4 diff = {light->getLightData().diffuse,          1.f};
        glm::vec4 spec = {light->getLightData().specular,         1.f};

        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &dir);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &amb);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &diff);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &spec);
        position += VEC4_SIZE;
    }
    if (nullLights > 0) {
        std::vector<byte> zeroes(4 * VEC4_SIZE * nullLights, 0);
        glBufferSubData(GL_UNIFORM_BUFFER, position, 4 * VEC4_SIZE, &zeroes[0]);
        position += 4 * VEC4_SIZE * nullLights;
    }

    nullLights = 0;
    for (int i = 0; i < POINT_LIGHT_COUNT; i++) {
        auto* light = pointLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        glm::vec4 pos =  {light->getGlobalPosition(),     0.f};
        glm::vec4 amb =  {light->getLightData().ambient,  1.f};
        glm::vec4 diff = {light->getLightData().diffuse,  1.f};
        glm::vec4 spec = {light->getLightData().specular, 1.f};
        glm::vec4 fall = {light->getLightData().constant, light->getLightData().linear, light->getLightData().quadratic, 0.f};

        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &pos);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &amb);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &diff);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &spec);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &fall);
        position += VEC4_SIZE;
    }
    if (nullLights > 0) {
        std::vector<byte> zeroes(5 * VEC4_SIZE * nullLights, 0);
        glBufferSubData(GL_UNIFORM_BUFFER, position, 5 * VEC4_SIZE, &zeroes[0]);
        position += 5 * VEC4_SIZE * nullLights;
    }

    nullLights = 0;
    for (int i = 0; i < SPOT_LIGHT_COUNT; i++) {
        auto* light = spotLights[i];
        if (!light) {
            nullLights++;
            continue;
        }

        glm::vec4 pos =  {light->getGlobalPosition(),             0.f};
        glm::vec4 dir =  {glm::eulerAngles(light->getRotation()), 0.f};
        glm::vec4 diff = {light->getLightData().diffuse,          1.f};
        glm::vec4 spec = {light->getLightData().specular,         1.f};
        glm::vec4 fall = {light->getLightData().constant,  light->getLightData().linear, light->getLightData().quadratic, 0.f};
        glm::vec4 cut  = {light->getLightData().innerCone, light->getLightData().outerCone, 0.f, 0.f};

        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &pos);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &dir);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &diff);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &spec);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &fall);
        position += VEC4_SIZE;
        glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &cut);
        position += VEC4_SIZE;
    }
    if (nullLights > 0) {
        std::vector<byte> zeroes(6 * VEC4_SIZE * nullLights, 0);
        glBufferSubData(GL_UNIFORM_BUFFER, position, 6 * VEC4_SIZE, &zeroes[0]);
        position += 6 * VEC4_SIZE * nullLights;
    }

    glm::vec4 counts{numberOfLights, 1.f};
    glBufferSubData(GL_UNIFORM_BUFFER, position, VEC4_SIZE, &counts);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
