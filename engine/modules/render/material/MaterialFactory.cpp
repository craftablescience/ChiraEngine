#include "MaterialFactory.h"

using namespace chira;

IMaterial::IMaterial(std::string path_)
        : Resource(std::move(path_)) {}

void IMaterial::compile(const std::byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

    this->shader = Resource::getResource<Shader>(this->shaderPath);
}

void IMaterial::use() const {
    this->shader->use();
}

SharedPointer<Shader> IMaterial::getShader() const {
    return this->shader;
}
