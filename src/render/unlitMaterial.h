#pragma once

#include "abstractMaterial.h"

class unlitMaterial : public abstractMaterial {
    void use() override {
        this->shader->use();
    }
};
