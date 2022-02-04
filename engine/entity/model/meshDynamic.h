#pragma once

#include <entity/entity.h>
#include <render/mesh/meshDataBuilder.h>

namespace chira {
    class MeshDynamic : public Entity {
    public:
        explicit MeshDynamic(const std::string& name_) : Entity(name_) {}
        MeshDynamic() : Entity() {}
        void render(glm::mat4 parentTransform) override;
        [[nodiscard]] MeshDataBuilder* getMesh();
    private:
        MeshDataBuilder mesh;
    };
}
