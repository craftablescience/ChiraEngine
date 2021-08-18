#include "abstractMaterial.h"

#include "../core/engine.h"
#include "../resource/resourceManager.h"

abstractMaterial::abstractMaterial(const std::string& shader_) {
    this->shaderName = shader_;
}

shader* abstractMaterial::getShader() {
    return engine::getShader(this->shaderName);
}
