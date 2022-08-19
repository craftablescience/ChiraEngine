#pragma once

#include <array>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace chira {

constexpr const int DIRECTIONAL_LIGHT_COUNT = 2;
constexpr const int POINT_LIGHT_COUNT = 22;
constexpr const int SPOT_LIGHT_COUNT = 8;

class LightManager {
    friend DirectionalLight;
    friend PointLight;
    friend SpotLight;
    friend class Engine;
    friend class Frame;
private:
    LightManager() = default;
    static void setupShaderMacros();

    void addLight(DirectionalLight* light);
    void removeLight(DirectionalLight* light);
    void addLight(PointLight* light);
    void removeLight(PointLight* light);
    void addLight(SpotLight* light);
    void removeLight(SpotLight* light);
    void updateUBOs();

    DirectionalLight* directionalLights[DIRECTIONAL_LIGHT_COUNT] {nullptr};
    unsigned short directionalLightCount = 0;
    PointLight* pointLights[POINT_LIGHT_COUNT] {nullptr};
    unsigned short pointLightCount = 0;
    SpotLight* spotLights[SPOT_LIGHT_COUNT] {nullptr};
    unsigned short spotLightCount = 0;
};

} // namespace chira
