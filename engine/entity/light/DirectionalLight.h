#pragma once

#include "../Entity.h"

namespace chira {

struct DirectionalLightData {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
};

class DirectionalLight : public Entity {
public:
    DirectionalLight(std::string name_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    void onAddedToTree() override;
    ~DirectionalLight() override;
    [[nodiscard]] const DirectionalLightData* getLightData() const;
    [[nodiscard]] glm::vec3 getAmbient() const;
    void setAmbient(glm::vec3 newAmbient);
    [[nodiscard]] glm::vec3 getDiffuse() const;
    void setDiffuse(glm::vec3 newDiffuse);
    [[nodiscard]] glm::vec3 getSpecular() const;
    void setSpecular(glm::vec3 newSpecular);
protected:
    DirectionalLightData data;
};

} // namespace chira
