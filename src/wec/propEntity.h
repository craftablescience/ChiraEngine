#pragma once

#include "abstractEntity.h"
#include "meshComponent.h"
#include "componentFactory.h"

namespace chira {
    class propEntity : public abstractEntity {
    public:
        propEntity* init(meshComponent* component) {
            this->add(component);
            return this;
        }
        void preRender(double delta) override {}
        void postRender(double delta) override {}
    private:
        RegisterComponentName(propEntity);
    };
}
