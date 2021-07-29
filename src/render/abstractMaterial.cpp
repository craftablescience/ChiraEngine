#include "abstractMaterial.h"

abstractMaterial::abstractMaterial(const std::string& shader_) {
    this->shader = shader_;
}

shader* abstractMaterial::getShader() {
    return engine::getShader(this->shader);
}
