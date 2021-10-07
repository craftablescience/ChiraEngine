#pragma once

#include "abstractEntity.h"

namespace chira {
    class abstractWorld : public abstractEntity {
    public:
        uuids::uuid add(abstractComponent* component) override;
    };
}
