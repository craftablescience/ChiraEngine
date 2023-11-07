#pragma once

#include <core/utility/UUIDGenerator.h>

namespace chira {

struct UUIDComponent {
    UUIDComponent()
            : uuid(UUIDGenerator::getNewUUID()) {}

    explicit UUIDComponent(uuids::uuid uuid_)
            : uuid(uuid_) {}

public:
    uuids::uuid uuid;
};

} // namespace chira
