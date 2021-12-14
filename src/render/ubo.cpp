#include "ubo.h"

#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace chira;

unsigned int ubo::uboBindingPoint = 0;

ubo::ubo(std::string name_, unsigned int size) : handleObject<unsigned int>(), name(std::move(name_)) {
    this->bindingPoint = ubo::uboBindingPoint++;
    glGenBuffers(1, &this->handle);
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, this->bindingPoint);
    glBindBufferRange(GL_UNIFORM_BUFFER, this->bindingPoint, this->handle, 0, size);
}

unsigned int ubo::getBindingPoint() const {
    return this->bindingPoint;
}

void ubo::bindToShader(shader* shader_) const {
    glUniformBlockBinding(shader_->getHandle(), glGetUniformBlockIndex(shader_->getHandle(), this->name.c_str()), this->bindingPoint);
}

void ubo::update(const unsigned char buffer[], GLsizeiptr length) {
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferData(GL_UNIFORM_BUFFER, length, buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::unique_ptr<uboPV> uboPV::singleton = std::unique_ptr<uboPV>();

uboPV::uboPV(const std::string& name) : ubo(name, 3 * sizeof(glm::mat4)) {}

uboPV* uboPV::get() {
    if (!uboPV::singleton)
        uboPV::singleton.reset(new uboPV{"PV"});
    return uboPV::singleton.get();
}

void uboPV::update(const glm::mat4& proj, const glm::mat4& view) {
    constexpr std::size_t mat4_size = sizeof(glm::mat4);
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * mat4_size, mat4_size, glm::value_ptr(proj));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * mat4_size, mat4_size, glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * mat4_size, mat4_size, glm::value_ptr(proj * view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
