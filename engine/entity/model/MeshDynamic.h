#pragma once

#include <entity/Entity.h>
#include <render/mesh/MeshDataBuilder.h>

namespace chira {

class MeshDynamic : public Entity {
public:
    explicit MeshDynamic(std::string name_) : Entity(std::move(name_)) {}
    void render(glm::mat4 parentTransform) override;
    [[nodiscard]] MeshDataBuilder* getMesh();
protected:
    MeshDataBuilder mesh;
};

} // namespace chira
