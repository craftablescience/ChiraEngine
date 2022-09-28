#pragma once

#include <concepts>
#include <unordered_map>
#include <string>
#include <string_view>
#include <glm/glm.hpp>
#include <core/Assertions.h>
#include <math/Matrix.h>

namespace chira {

class Group;
class Frame;

/// The base entity class. Note that the name of an entity stored in the name variable should
/// match the name assigned to the entity in the parent's entity map.
/// Note the entity tree must have a Window as the root entity, always!
/// Window objects are Frame objects, Frame objects are Root objects (the wonders of inheritance)
class Entity {
public:
    explicit Entity(std::string name_);
    /// Initializes name to a random UUID.
    Entity();
    virtual ~Entity();

    /// Run game logic.
    virtual void update();

    /// Draw to screen.
    virtual void render(glm::mat4 parentTransform);

    [[nodiscard]] virtual const Frame* getFrame() const;
    [[nodiscard]] virtual Frame* getFrame();
    [[nodiscard]] virtual const Group* getGroup() const;
    [[nodiscard]] virtual Group* getGroup();

    [[nodiscard]] Entity* getParent() const;
    [[nodiscard]] std::string_view getName() const;

    [[nodiscard]] Entity* getChild(std::string_view name_) const;
    template<typename T>
    requires std::derived_from<T, Entity>
    [[nodiscard]] T* getChild(std::string_view name_) const {
        return assert_cast<T*>(this->getChild(name_));
    }
    [[nodiscard]] bool hasChild(std::string_view name_) const;
    virtual std::string_view addChild(Entity* child);
    virtual void removeChild(std::string_view name_);
    void removeAllChildren();

    [[nodiscard]] bool isVisible() const;
    virtual void setVisible(bool visibility);
    virtual void setPosition(glm::vec3 newPos);
    virtual void setRotation(glm::quat newRot);
    [[nodiscard]] virtual glm::vec3 getPosition();
    [[nodiscard]] virtual glm::vec3 getGlobalPosition();
    /// Note: the global rotation is inaccessible.
    [[nodiscard]] virtual glm::quat getRotation();
    virtual void translate(glm::vec3 translateByAmount);
    virtual void translateWithRotation(glm::vec3 translateByAmount);
    virtual void rotate(glm::quat rotateByAmount);
    virtual void rotate(glm::vec3 rotateByAmount);
protected:
    Entity* parent = nullptr;
    std::string name;
    std::vector<Entity*> children;
    bool visible = true;

    // The following are in local space and are relative to the parent.
    glm::vec3 position{};
    glm::quat rotation = glm::identity<glm::quat>();

    /// For internal use only!
    void setParent(Entity* newParent) {
        this->parent = newParent;
    }

    /// Callback called after parent is set
    virtual void onAddedToTree() {}
};

} // namespace chira
