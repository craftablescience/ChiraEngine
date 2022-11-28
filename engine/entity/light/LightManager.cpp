#include "LightManager.h"

#include <core/Logger.h>
#include <resource/ShaderResource.h>
#include <render/shader/UBO.h>

using namespace chira;

CHIRA_CREATE_LOG(LIGHTMANAGER);

void LightManager::setupShaderMacros() {
    ShaderResource::addPreprocessorSymbol("DIRECTIONAL_LIGHT_COUNT", std::to_string(DIRECTIONAL_LIGHT_COUNT));
    ShaderResource::addPreprocessorSymbol("POINT_LIGHT_COUNT", std::to_string(POINT_LIGHT_COUNT));
    ShaderResource::addPreprocessorSymbol("SPOT_LIGHT_COUNT", std::to_string(SPOT_LIGHT_COUNT));
}

void LightManager::addLight(DirectionalLight* light) {
    for (auto& directionalLight : this->directionalLights) {
        if (!directionalLight) {
            directionalLight = light;
            this->directionalLightCount++;
            return;
        }
    }
    LOG_LIGHTMANAGER.warning("Too many directional lights! Additional lights will not be displayed");
}

void LightManager::removeLight(DirectionalLight* light) {
    for (auto& directionalLight : this->directionalLights) {
        if (directionalLight == light) {
            directionalLight = nullptr;
            this->directionalLightCount--;
            return;
        }
    }
}

void LightManager::addLight(PointLight* light) {
    for (auto& pointLight : this->pointLights) {
        if (!pointLight) {
            pointLight = light;
            this->pointLightCount++;
            return;
        }
    }
    LOG_LIGHTMANAGER.warning("Too many point lights! Additional lights will not be displayed");
}

void LightManager::removeLight(PointLight* light) {
    for (auto& pointLight : this->pointLights) {
        if (pointLight == light) {
            pointLight = nullptr;
            this->pointLightCount--;
            return;
        }
    }
}

void LightManager::addLight(SpotLight* light) {
    for (auto& spotLight : this->spotLights) {
        if (!spotLight) {
            spotLight = light;
            this->spotLightCount++;
            return;
        }
    }
    LOG_LIGHTMANAGER.warning("Too many spotlights! Additional lights will not be displayed");
}

void LightManager::removeLight(SpotLight* light) {
    for (auto& spotLight : this->spotLights) {
        if (spotLight == light) {
            spotLight = nullptr;
            this->spotLightCount--;
            return;
        }
    }
}

void LightManager::updateUBOs() {
    LightsUBO::get()->updateLights(this->directionalLights, this->pointLights, this->spotLights, {this->directionalLightCount, this->pointLightCount, this->spotLightCount});
}
