#pragma once

#include <entity/entity.h>
#include <render/mesh/meshDataResource.h>

namespace chira {
    class Mesh : public Entity {
    public:
        Mesh(Entity* parent_, const std::string& meshId) : Entity(parent_) {
            this->mesh = Resource::getResource<MeshDataResource>(meshId);
        }
        Mesh(Entity* parent_, const std::string& name_, const std::string& meshId) : Entity(parent_, name_) {
            this->mesh = Resource::getResource<MeshDataResource>(meshId);
        }
        explicit Mesh(const std::string& meshId) : Entity() {
            this->mesh = Resource::getResource<MeshDataResource>(meshId);
        }
        Mesh(const std::string& name_, const std::string& meshId) : Entity(name_) {
            this->mesh = Resource::getResource<MeshDataResource>(meshId);
        }
        void render(glm::mat4 parentTransform) override;
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
