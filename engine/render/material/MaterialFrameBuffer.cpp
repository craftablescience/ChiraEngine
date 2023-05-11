#include "MaterialFrameBuffer.h"

using namespace chira;

void MaterialFrameBuffer::compile(const byte buffer[], std::size_t bufferLength) {
    IMaterial::compile(buffer, bufferLength);

    this->shader = Resource::getResource<Shader>(this->shaderPath);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}

void MaterialFrameBuffer::use() const {
    IMaterial::use();
    Renderer::useFrameBufferTexture(*this->handle, TextureUnit::G0);
}
