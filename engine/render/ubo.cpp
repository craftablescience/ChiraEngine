#include "ubo.h"

#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace chira;

unsigned int UBO::uboBindingPoint = 0;

UBO::UBO(std::string name_, unsigned int size) : HandleObject<unsigned int>(), name(std::move(name_)) {
    this->bindingPoint = UBO::uboBindingPoint++;
    glGenBuffers(1, &this->handle);
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, this->bindingPoint);
    glBindBufferRange(GL_UNIFORM_BUFFER, this->bindingPoint, this->handle, 0, size);
}

unsigned int UBO::getBindingPoint() const {
    return this->bindingPoint;
}

void UBO::bindToShader(Shader* shader_) const {
    glUniformBlockBinding(shader_->getHandle(), glGetUniformBlockIndex(shader_->getHandle(), this->name.c_str()), this->bindingPoint);
}

void UBO::update(const unsigned char buffer[], GLsizeiptr length) {
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferData(GL_UNIFORM_BUFFER, length, buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::unique_ptr<UBO_PV> UBO_PV::singleton = std::unique_ptr<UBO_PV>();

UBO_PV::UBO_PV(const std::string& name) : UBO(name, 3 * sizeof(glm::mat4)) {}

UBO_PV* UBO_PV::get() {
    if (!UBO_PV::singleton)
        UBO_PV::singleton.reset(new UBO_PV{"PV"});
    return UBO_PV::singleton.get();
}

void UBO_PV::update(const glm::mat4& proj, const glm::mat4& view) {
    constexpr std::size_t mat4_size = sizeof(glm::mat4);
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * mat4_size, mat4_size, glm::value_ptr(proj));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * mat4_size, mat4_size, glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * mat4_size, mat4_size, glm::value_ptr(proj * view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
