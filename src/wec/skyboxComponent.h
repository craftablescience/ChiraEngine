#pragma once

#include "../render/textureCubemap.h"
#include "../resource/meshResource.h"
#include "abstractComponent.h"

namespace chira {
    class skyboxComponent : public abstractComponent {
    public:
        explicit skyboxComponent(const std::string& materialId);
        void render(double delta) override;
        ~skyboxComponent() override;
        [[nodiscard]] meshResource* getMesh() const {
            return skyboxMesh;
        }
    private:
        meshResource* skyboxMesh = nullptr;
    };
}
