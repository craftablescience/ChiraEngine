#include "MaterialFactory.h"

using namespace chira;

IMaterial::IMaterial(std::string identifier_)
        : Resource(std::move(identifier_)) {}

void IMaterial::compile(const byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

    this->shader = Resource::getResource<Shader>(this->shaderPath);
}

void IMaterial::use() const {
    this->shader->use();
}

SharedPointer<Shader> IMaterial::getShader() const {
    return this->shader;
}
