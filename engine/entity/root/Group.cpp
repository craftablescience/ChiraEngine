#include "Group.h"

using namespace chira;

Group::Group(std::string name_) : Entity(std::move(name_)) {}

const Group* Group::getGroup() const {
    return this;
}

Group* Group::getGroup() {
    return this;
}
