#pragma once

#include <entt/entt.hpp>

#include <core/debug/Assertions.h>
#include <core/math/Types.h>
#include <core/utility/Tuple.h>
#include "LayerComponents.h"
#include "TransformComponent.h"

namespace chira {

struct CameraComponent {
    enum class ProjectionMode {
        PERSPECTIVE,
        ORTHOGRAPHIC,
    };

    explicit CameraComponent(ProjectionMode mode = ProjectionMode::PERSPECTIVE, float fov_ = 70.f,
                             float nearDistance_ = 0.1f, float farDistance_ = 1024.f, float orthoSize_ = 10.f,
                             bool active_ = true)
            : projectionMode(mode)
            , activeLayers(0)
            , fov(fov_)
            , nearDistance(nearDistance_)
            , farDistance(farDistance_)
            , orthoSize(orthoSize_)
            , active(active_) {
        foreach(LAYER_COMPONENTS, [&](auto layer) {
            this->activeLayers += decltype(layer)::index;
        });
    }

    [[nodiscard]] glm::mat4 getProjection(glm::vec2i size) const {
        size.x = std::max(size.x, 1);
        size.y = std::max(size.y, 1);
        float aspectRatio = static_cast<float>(size.x) / static_cast<float>(size.y);

        switch (this->projectionMode) {
            using enum ProjectionMode;
            case PERSPECTIVE: {
                return glm::perspective(glm::radians(this->fov), aspectRatio, this->nearDistance, this->farDistance);
            }
            case ORTHOGRAPHIC: {
                float orthoRight = this->orthoSize * aspectRatio / 2;
                float orthoLeft = -orthoRight;
                float orthoTop = this->orthoSize / 2;
                float orthoBottom = -orthoTop;
                return glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, this->nearDistance, this->farDistance);
            }
            CHIRA_NO_DEFAULT;
        }
    }

    [[nodiscard]] glm::mat4 getView() const {
        const glm::vec3 position = this->transform->getPosition();
        return glm::lookAt(position, position + this->transform->getFrontVector(), this->transform->getUpVector());
    }

    void setCurrent() {
        this->active = true;
    }

    void clearCurrent() {
        this->active = false;
    }

public:
    TransformComponent* transform = nullptr;
    ProjectionMode projectionMode;

    // The combined values of every layer index
    std::size_t activeLayers;

    float fov;
    float nearDistance;
    float farDistance;

    float orthoSize;

    bool active;
};

} // namespace chira
