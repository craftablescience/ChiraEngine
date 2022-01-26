#pragma once

#include <entity/entity.h>
#include <render/mesh/meshDataBuilder.h>

namespace chira {
    class MeshDynamic : public Entity {
    public:
        explicit MeshDynamic(Entity* parent_) : Entity(parent_) {}
        MeshDynamic(Entity* parent_, const std::string& name_) : Entity(parent_, name_) {}
        MeshDynamic() : Entity() {}
        explicit MeshDynamic(const std::string& name_) : Entity(name_) {}
        void render(glm::mat4 parentTransform) override;
        glm::vec3 getAABB() const override;
        [[nodiscard]] MeshDataBuilder* getMesh();
    private:
        MeshDataBuilder mesh;
    };
}
