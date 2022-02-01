#include "ubo.h"

#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace chira;

unsigned int UniformBufferObject::uboBindingPoint = 0;

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

void UniformBufferObject::update(const unsigned char buffer[], GLsizeiptr length) const {
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferData(GL_UNIFORM_BUFFER, length, buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::unique_ptr<UBO_PerspectiveView> UBO_PerspectiveView::singleton = std::unique_ptr<UBO_PerspectiveView>();

UBO_PerspectiveView::UBO_PerspectiveView() : UniformBufferObject("PV", 3 * sizeof(glm::mat4)) {}

UBO_PerspectiveView* UBO_PerspectiveView::get() {
    if (!UBO_PerspectiveView::singleton)
        UBO_PerspectiveView::singleton.reset(new UBO_PerspectiveView{});
    return UBO_PerspectiveView::singleton.get();
}

void UBO_PerspectiveView::update(const glm::mat4& proj, const glm::mat4& view) const {
    constexpr std::size_t mat4_size = sizeof(glm::mat4);
    glBindBuffer(GL_UNIFORM_BUFFER, this->handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * mat4_size, mat4_size, glm::value_ptr(proj));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * mat4_size, mat4_size, glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * mat4_size, mat4_size, glm::value_ptr(proj * view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
