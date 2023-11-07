#pragma once

#include <concepts>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace chira {

struct TransformComponent {
    static constexpr auto in_place_delete = true;

    explicit TransformComponent(TransformComponent* parent_ = nullptr,
                                glm::vec3 position_ = {},
                                glm::quat rotation_ = glm::identity<glm::quat>(),
                                glm::vec3 scale_ = glm::vec3{1})
            : transform(glm::identity<glm::mat4>())
            , rotationQuat(rotation_)
            , rotationEuler(glm::eulerAngles(rotation_))
            , position(position_)
            , scale(scale_)
            , parent(parent_)
            , useEulerAngles(false)
            , dirty(true) {}

    [[nodiscard]] glm::mat4 getMatrix() { // NOLINT(misc-no-recursion)
        if (this->parent) {
            return this->parent->getMatrix() * this->getMatrixLocal();
        }
        return this->getMatrixLocal();
    }
    [[nodiscard]] const glm::mat4& getMatrixLocal() {
        if (dirty) {
            this->transform = TransformComponent::createTransformMatrix(
                    glm::identity<glm::mat4>(),
                    this->getPositionLocal(),
                    this->getRotation(),
                    this->getScale()
            );
            this->dirty = false;
        }
        return this->transform;
    }
    void setMatrixLocal(const glm::mat4& transform_) {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform_, this->scale, this->rotationQuat, this->position, skew, perspective);
        // Fix euler angles
        this->setRotation(this->rotationQuat);
    }

    [[nodiscard]] glm::vec3 getPosition() const { // NOLINT(misc-no-recursion)
        if (this->parent) {
            return this->parent->getPosition() + this->getPositionLocal();
        }
        return this->position;
    }
    void setPosition(glm::vec3 newGlobalPosition) { // NOLINT(misc-no-recursion)
        if (this->parent) {
            this->position = newGlobalPosition - this->parent->getPosition();
        } else {
            this->position = newGlobalPosition;
        }
        this->dirty = true;
    }

    [[nodiscard]] glm::vec3 getPositionLocal() const {
        return this->position;
    }
    void setPositionLocal(glm::vec3 newLocalPosition) {
        this->position = newLocalPosition;
        this->dirty = true;
    }

    [[nodiscard]] glm::quat getRotation() const {
        return this->useEulerAngles ? glm::quat(this->rotationEuler) : this->rotationQuat;
    }
    [[nodiscard]] glm::vec3 getRotationEuler() const {
        return this->useEulerAngles ? this->rotationEuler : glm::eulerAngles(this->rotationQuat);
    }
    /// Returns an angle in radians.
    [[nodiscard]] float getPitch() const {
        return this->getRotationEuler().x;
    }
    /// Returns an angle in radians.
    [[nodiscard]] float getYaw() const {
        return this->getRotationEuler().y;
    }
    /// Returns an angle in radians.
    [[nodiscard]] float getRoll() const {
        return this->getRotationEuler().z;
    }
    void setRotation(glm::quat newRotation) {
        this->rotationQuat = newRotation;
        this->rotationEuler = glm::eulerAngles(newRotation);
        this->useEulerAngles = false;
        this->dirty = true;
    }
    void setRotation(glm::vec3 newRotation) {
        this->rotationQuat = glm::quat(newRotation);
        this->rotationEuler = newRotation;
        this->useEulerAngles = true;
        this->dirty = true;
    }
    /// Takes an angle in radians.
    void setPitch(float pitch) {
        this->setRotation({pitch, this->getYaw(), this->getRoll()});
        this->useEulerAngles = true;
        this->dirty = true;
    }
    /// Takes an angle in radians.
    void setYaw(float yaw) {
        this->setRotation({this->getPitch(), yaw, this->getRoll()});
        this->useEulerAngles = true;
        this->dirty = true;
    }
    /// Takes an angle in radians.
    void setRoll(float roll) {
        this->setRotation({this->getPitch(), this->getYaw(), roll});
        this->useEulerAngles = true;
        this->dirty = true;
    }

    [[nodiscard]] glm::vec3 getScale() const {
        return this->scale;
    }
    void setScale(glm::vec3 newScale) {
        this->scale = newScale;
        this->dirty = true;
    }

    void translate(glm::vec3 translateByAmount) {
        this->position += translateByAmount;
        this->dirty = true;
    }
    void translateWithRotation(glm::vec3 translateByAmount) {
        glm::quat p{glm::length(translateByAmount), translateByAmount.x, translateByAmount.y, translateByAmount.z};
        p = this->getRotation() * p * glm::conjugate(this->getRotation());
        this->translate(glm::vec3{p.x, p.y, p.z});
    }

    void rotate(glm::quat rotateByAmount) {
        this->rotationQuat += rotateByAmount;
        this->rotationEuler = glm::eulerAngles(this->rotationQuat);
        this->useEulerAngles = false;
        this->dirty = true;
    }
    void rotate(glm::vec3 rotateByAmount) {
        this->pitch(rotateByAmount.x);
        this->yaw(rotateByAmount.y);
        this->roll(rotateByAmount.z);
        this->rotationQuat = glm::quat(this->rotationEuler);
        this->useEulerAngles = true;
        this->dirty = true;
    }
    void pitch(float pitch) {
        this->rotationEuler = glm::rotate(glm::angleAxis(pitch, glm::vec3{1,0,0}), this->rotationEuler);
        this->rotationQuat = glm::quat(this->rotationEuler);
        this->useEulerAngles = true;
        this->dirty = true;
    }
    void yaw(float yaw) {
        this->rotationEuler = glm::rotate(glm::angleAxis(yaw, glm::vec3{0,1,0}), this->rotationEuler);
        this->rotationQuat = glm::quat(this->rotationEuler);
        this->useEulerAngles = true;
        this->dirty = true;
    }
    void roll(float roll) {
        this->rotationEuler = glm::rotate(glm::angleAxis(roll, glm::vec3{0,0,1}), this->rotationEuler);
        this->rotationQuat = glm::quat(this->rotationEuler);
        this->useEulerAngles = true;
        this->dirty = true;
    }

    [[nodiscard]] glm::vec3 getFrontVector() const {
        glm::vec3 out;
        out.x =     2 * (this->rotationQuat.x * this->rotationQuat.z + this->rotationQuat.w * this->rotationQuat.y);
        out.y =     2 * (this->rotationQuat.y * this->rotationQuat.z - this->rotationQuat.w * this->rotationQuat.x);
        out.z = 1 - 2 * (this->rotationQuat.x * this->rotationQuat.x + this->rotationQuat.y * this->rotationQuat.y);
        return -out;
    }
    [[nodiscard]] glm::vec3 getUpVector() const {
        glm::vec3 out;
        out.x =     2 * (this->rotationQuat.x * this->rotationQuat.y - this->rotationQuat.w * this->rotationQuat.z);
        out.y = 1 - 2 * (this->rotationQuat.x * this->rotationQuat.x + this->rotationQuat.z * this->rotationQuat.z);
        out.z =     2 * (this->rotationQuat.y * this->rotationQuat.z + this->rotationQuat.w * this->rotationQuat.x);
        return out;
    }
    [[nodiscard]] glm::vec3 getRightVector() const {
        glm::vec3 out;
        out.x = 1 - 2 * (this->rotationQuat.y * this->rotationQuat.y + this->rotationQuat.z * this->rotationQuat.z);
        out.y =     2 * (this->rotationQuat.x * this->rotationQuat.y + this->rotationQuat.w * this->rotationQuat.z);
        out.z =     2 * (this->rotationQuat.x * this->rotationQuat.z - this->rotationQuat.w * this->rotationQuat.y);
        return out;
    }

    [[nodiscard]] bool operator==(const TransformComponent& other) const {
        return this->parent == other.parent &&
               this->position == other.position &&
               this->rotationQuat == other.rotationQuat &&
               this->scale == other.scale;
    }

    [[nodiscard]] static glm::mat4 createTransformMatrix(const glm::mat4& start, glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
        return glm::scale(glm::translate(start, position) * glm::mat4_cast(rotation), scale);
    }

protected:
    glm::mat4 transform;
    glm::quat rotationQuat;
    glm::vec3 rotationEuler;
    glm::vec3 position;
    glm::vec3 scale;

    TransformComponent* parent;
    bool useEulerAngles;
    bool dirty;
};

template<typename T>
concept CComponentHasTransform = requires(T t) {
    // no i don't know why it needs the & at the end but it does
    {t.transform} -> std::same_as<TransformComponent*&>;
};

} // namespace chira
