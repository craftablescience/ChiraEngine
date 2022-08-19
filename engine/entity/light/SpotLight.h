#pragma once

#include "../Entity.h"

namespace chira {

struct SpotLightData {
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float innerCone;
    float outerCone;
};

class SpotLight : public Entity {
public:
    explicit SpotLight(std::string name_);
    SpotLight();
    void onAddedToTree() override;
    ~SpotLight() override;
    [[nodiscard]] SpotLightData& getLightData() {
        return this->data;
    }
protected:
    SpotLightData data{};
};

} // namespace chira
