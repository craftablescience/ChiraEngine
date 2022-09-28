#pragma once

#include "../Entity.h"

namespace chira {

struct PointLightData {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 falloff;
};

class PointLight : public Entity {
public:
    PointLight(std::string name_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 falloff);
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 falloff);
    void onAddedToTree() override;
    ~PointLight() override;
    [[nodiscard]] const PointLightData* getLightData() const;
    [[nodiscard]] glm::vec3 getAmbient() const;
    void setAmbient(glm::vec3 newAmbient);
    [[nodiscard]] glm::vec3 getDiffuse() const;
    void setDiffuse(glm::vec3 newDiffuse);
    [[nodiscard]] glm::vec3 getSpecular() const;
    void setSpecular(glm::vec3 newSpecular);
    [[nodiscard]] float getConstant() const;
    void setConstant(float newConstant);
    [[nodiscard]] float getLinear() const;
    void setLinear(float newLinear);
    [[nodiscard]] float getQuadratic() const;
    void setQuadratic(float newQuadratic);
protected:
    PointLightData data;
};

} // namespace chira
