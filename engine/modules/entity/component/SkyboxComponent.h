#pragma once

#include <render/material/MaterialCubemap.h>
#include <render/mesh/MeshDataBuilder.h>

namespace chira {

struct SkyboxComponent {
    explicit SkyboxComponent(const std::string& cubemapId = "materials/skybox.json") {
        this->skybox.addCube({}, {1, 1, 1}, false);
        this->skybox.update();
        this->skybox.setMaterial(Resource::getResource<MaterialCubemap>(cubemapId).cast<IMaterial>());
    }

public:
    MeshDataBuilder skybox{};
};

} // namespace chira
