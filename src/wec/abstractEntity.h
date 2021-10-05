#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "abstractComponent.h"
#include "../utility/uuidGenerator.h"

namespace chira {
    class abstractEntity : public abstractComponent {
    public:
        virtual void preRender(double delta) = 0;
        void render(double delta) override {}
        virtual void postRender(double delta) = 0;
        uuids::uuid add(abstractComponent* component);
        abstractComponent* get(const uuids::uuid& uuid);
        void remove(const uuids::uuid& uuid);
        ~abstractEntity() override;
    protected:
        std::unordered_map<uuids::uuid,abstractComponent*> components;
    };
}
