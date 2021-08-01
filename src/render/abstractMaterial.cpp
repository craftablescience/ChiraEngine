#include "abstractMaterial.h"

abstractMaterial::abstractMaterial(const std::string& shader_) {
    this->shaderName = shader_;
}

shader* abstractMaterial::getShader() {
    return engine::getShader(this->shaderName);
}
