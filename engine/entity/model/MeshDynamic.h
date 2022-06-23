#pragma once

#include <entity/Entity.h>
#include <render/mesh/MeshDataBuilder.h>

namespace chira {

class MeshDynamic : public Entity {
public:
    explicit MeshDynamic(const std::string& name_) : Entity(name_) {}
    MeshDynamic() : Entity() {}
    void render(glm::mat4 parentTransform) override;
    [[nodiscard]] MeshDataBuilder* getMesh();
protected:
    MeshDataBuilder mesh;
};

}
