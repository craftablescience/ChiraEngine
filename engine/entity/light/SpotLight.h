#pragma once

#include "../Entity.h"

namespace chira {

struct SpotLightData {
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 falloff;
    glm::vec4 cutoff;
};

class SpotLight : public Entity {
public:
    SpotLight(std::string name_, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 falloff, glm::vec2 cutoff);
    void onAddedToTree() override;
    ~SpotLight() override;
    [[nodiscard]] const SpotLightData* getLightData() const;
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
    [[nodiscard]] float getInnerCone() const;
    void setInnerCone(float newInnerCone);
    [[nodiscard]] float getOuterCone() const;
    void setOuterCone(float newOuterCone);
protected:
    SpotLightData data;
};

} // namespace chira
