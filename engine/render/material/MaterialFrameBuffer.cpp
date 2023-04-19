#include "MaterialFrameBuffer.h"

using namespace chira;

void MaterialFrameBuffer::compile(const nlohmann::json& properties) {
    Reflect::fromJSON(this, properties);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}

void MaterialFrameBuffer::use() const {
    IMaterial::use();
    Renderer::useFrameBufferTexture(*this->handle, TextureUnit::G0);
}
