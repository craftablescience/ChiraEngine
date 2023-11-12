#include "MaterialFrameBuffer.h"

using namespace chira;

void MaterialFrameBuffer::compile(const std::byte buffer[], std::size_t bufferLength) {
    IMaterial::compile(buffer, bufferLength);

    this->shader->use();
    this->shader->setUniform("texture0", 0);
}

void MaterialFrameBuffer::use() const {
    IMaterial::use();
    Renderer::useFrameBufferTexture(*this->handle, TextureUnit::G0);
}
