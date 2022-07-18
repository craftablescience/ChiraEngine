#pragma once

#include <entity/Entity.h>
#include <render/mesh/MeshDataResource.h>

namespace chira {

class Mesh : public Entity {
public:
    Mesh(std::string name_, const std::string& meshId) : Entity(std::move(name_)) {
        this->mesh = Resource::getResource<MeshDataResource>(meshId);
    }
    explicit Mesh(const std::string& meshId) : Entity() {
        this->mesh = Resource::getResource<MeshDataResource>(meshId);
    }
    void update(glm::mat4 parentTransform) override;
    [[nodiscard]] SharedPointer<MeshDataResource> getMeshResource() const {
        return this->mesh;
    }
    [[nodiscard]] std::vector<byte> getMeshData(const std::string& meshLoader) const {
        return this->mesh->getMeshData(meshLoader);
    }
private:
    SharedPointer<MeshDataResource> mesh;
};

}
