#include "MaterialFramebuffer.h"

#include <entity/root/Frame.h>

using namespace chira;

void MaterialFramebuffer::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}

void MaterialFramebuffer::use() const {
    IMaterial::use();
    this->frame->useFrameBufferTexture();
}
