#pragma once

#include <entity/Entity.h>

namespace chira {

/// The Group is an in-world entity that is purely symbolic (for now at least).
class Group : public Entity {
public:
    explicit Group(std::string name_);
    [[nodiscard]] const Group* getGroup() const override;
    [[nodiscard]] Group* getGroup() override;
};

} // namespace chira
