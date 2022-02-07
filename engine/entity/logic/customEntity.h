#pragma once

#include <functional>
#include <utility>
#include <entity/entity.h>

namespace chira {
    class CustomEntity : public Entity {
    public:
        CustomEntity(const std::string& name_, std::function<void(glm::mat4)> callback_)
            : Entity(name_)
            , callback(std::move(callback_)) {}
        explicit CustomEntity(std::function<void(glm::mat4)> callback_)
            : Entity()
            , callback(std::move(callback_)) {}
        void render(glm::mat4 parentTransform) override {
            this->callback(std::forward<glm::mat4>(parentTransform));
        }
    protected:
        std::function<void(glm::mat4)> callback;
    };
}
