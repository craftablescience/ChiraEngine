#pragma once

#include <utility>

#include <entity/3d/entity3d.h>
#include <resource/meshResource.h>

namespace chira {
    class mesh3d : public entity3d {
    public:
        mesh3d(entity* parent_, sharedPointer<meshResource> mesh_) : entity3d(parent_), mesh(std::move(mesh_)) {}
        mesh3d(entity* parent_, const std::string& name_, sharedPointer<meshResource> mesh_) : entity3d(parent_, name_), mesh(std::move(mesh_)) {}
        explicit mesh3d(sharedPointer<meshResource> mesh_) : entity3d(), mesh(std::move(mesh_)) {}
        mesh3d(const std::string& name_, sharedPointer<meshResource> mesh_) : entity3d(name_), mesh(std::move(mesh_)) {}
        void render(const glm::mat4& parentTransform) override;
        sharedPointer<meshResource> getMeshResource() const {
            return this->mesh;
        }
    private:
        sharedPointer<meshResource> mesh;
    };
}
