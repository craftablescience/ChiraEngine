#pragma once

#include "../Entity.h"

namespace chira {

struct PointLightData {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

class PointLight : public Entity {
public:
    explicit PointLight(std::string name_);
    PointLight();
    void onAddedToTree() override;
    ~PointLight() override;
    [[nodiscard]] PointLightData& getLightData() {
        return this->data;
    }
protected:
    PointLightData data{};
};

} // namespace chira
