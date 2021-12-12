#pragma once

#include <utility>

#include <entity/3d/entity3d.h>
#include <resource/meshResource.h>

namespace chira {
    class mesh3d : public entity3d {
    public:
        mesh3d(entity* parent_, meshResource* mesh_) : entity3d(parent_), mesh(mesh_) {}
        mesh3d(entity* parent_, const std::string& name_, meshResource* mesh_) : entity3d(parent_, name_), mesh(mesh_) {}
        explicit mesh3d(sharedPointer<meshResource> mesh_) : entity3d(), mesh(std::move(mesh_)) {}
        mesh3d(const std::string& name_, meshResource* mesh_) : entity3d(name_), mesh(mesh_) {}
        ~mesh3d() override;
        void render(const glm::mat4& parentTransform) override;
        sharedPointer<meshResource> getMeshResource() const {
            return this->mesh;
        }
    private:
        sharedPointer<meshResource> mesh;
    };
}
