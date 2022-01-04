#pragma once

#include <utility>
#include "../entity3d.h"
#include <resource/meshResource.h>

namespace chira {
    class Mesh3d : public Entity3d {
    public:
        Mesh3d(Entity* parent_, SharedPointer<MeshResource> mesh_) : Entity3d(parent_), mesh(std::move(mesh_)) {}
        Mesh3d(Entity* parent_, const std::string& name_, SharedPointer<MeshResource> mesh_) : Entity3d(parent_, name_), mesh(std::move(mesh_)) {}
        explicit Mesh3d(SharedPointer<MeshResource> mesh_) : Entity3d(), mesh(std::move(mesh_)) {}
        Mesh3d(const std::string& name_, SharedPointer<MeshResource> mesh_) : Entity3d(name_), mesh(std::move(mesh_)) {}
        void render(const glm::mat4& parentTransform) override;
        [[nodiscard]] SharedPointer<MeshResource> getMeshResource() const {
            return this->mesh;
        }
    private:
        SharedPointer<MeshResource> mesh;
    };
}
