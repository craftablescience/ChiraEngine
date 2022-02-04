#include "materialFramebuffer.h"

#include <entity/root/frame.h>

using namespace chira;

void MaterialFramebuffer::compile(const nlohmann::json& properties) {
    MaterialUntextured::compile(properties);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}

void MaterialFramebuffer::use() const {
    MaterialUntextured::use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->frame->getColorTextureHandle());
}
