#include "MaterialUntextured.h"

using namespace chira;

void MaterialUntextured::compile(const std::byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

    this->shader = Resource::getResource<Shader>(this->shaderPath);
}
