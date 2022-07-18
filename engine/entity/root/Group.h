#pragma once

#include <entity/Entity.h>
#include <entity/camera/Camera.h>
#include <render/mesh/MeshDataBuilder.h>
#include <render/material/MaterialCubemap.h>

namespace chira {

class Camera;

/// The Root is an in-world entity. It is designed to contain a collection of entities, like a "map".
/// It will never be the true root of the scene, that honor goes to a Window.
class Group : public Entity {
    friend class Engine;
public:
    explicit Group(std::string name_);
    Group();
    void update(glm::mat4 parentTransform) override;
    [[nodiscard]] glm::vec3 getGlobalPosition() override;
    [[nodiscard]] const Group* getGroup() const override;
    [[nodiscard]] Group* getGroup() override;
    void setCamera(Camera* camera);
    [[nodiscard]] Camera* getCamera() const;
    void setSkybox(const std::string& cubemapId);
    [[nodiscard]] SharedPointer<MaterialCubemap> getSkybox() const;
protected:
    MeshDataBuilder skybox;
    bool renderSkybox = false;
    bool skyboxMeshCreated = false;
    Camera* mainCamera = nullptr;
};

}
