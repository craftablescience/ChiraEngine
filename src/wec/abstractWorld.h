#pragma once

#include "abstractEntity.h"

namespace chira {
    class abstractWorld : public abstractEntity {
    public:
        using abstractEntity::add;
        uuids::uuid add(abstractEntity* entity);
    };
}
