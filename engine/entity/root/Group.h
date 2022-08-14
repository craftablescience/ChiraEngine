#pragma once

#include <entity/Entity.h>


namespace chira {

class Camera;

/// The Group is an in-world entity. It is designed to contain a collection of entities, like the root of a scene.
/// It will never be the true root of the scene, that honor goes to a Window.
class Group : public Entity {
    friend class Engine;
public:
    explicit Group(std::string name_);
    Group();
    [[nodiscard]] const Group* getGroup() const override;
    [[nodiscard]] Group* getGroup() override;
    [[nodiscard]] glm::vec3 getGlobalPosition() const override;
};

} // namespace chira
