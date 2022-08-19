#pragma once

#include "../Entity.h"

namespace chira {

struct DirectionalLightData {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class DirectionalLight : public Entity {
public:
    explicit DirectionalLight(std::string name_);
    DirectionalLight();
    void onAddedToTree() override;
    ~DirectionalLight() override;
    [[nodiscard]] DirectionalLightData& getLightData() {
        return this->data;
    }
protected:
    DirectionalLightData data{};
};

} // namespace chira
