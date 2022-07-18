#pragma once

#include <entity/Entity.h>
#include <render/mesh/MeshDataBuilder.h>

namespace chira {

class MeshDynamic : public Entity {
public:
    explicit MeshDynamic(std::string name_) : Entity(std::move(name_)) {}
    MeshDynamic() : Entity() {}
    void update(glm::mat4 parentTransform) override;
    [[nodiscard]] MeshDataBuilder* getMesh();
protected:
    MeshDataBuilder mesh;
};

}
