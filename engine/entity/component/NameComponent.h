#pragma once

#include <string>

namespace chira {

struct NameComponent {
    explicit NameComponent(std::string name_)
            : name(std::move(name_)) {}

public:
    std::string name;
};

} // namespace chira
