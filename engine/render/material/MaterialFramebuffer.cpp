#include "MaterialFramebuffer.h"

// todo(render): move to render backend
#include <glad/gl.h>
#include <entity/root/Frame.h>

using namespace chira;

void MaterialFramebuffer::compile(const nlohmann::json& properties) {
    IMaterial::compile(properties);
    this->shader->use();
    this->shader->setUniform("texture0", 0);
}

void MaterialFramebuffer::use() const {
    IMaterial::use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->frame->getColorTextureHandle());
}
