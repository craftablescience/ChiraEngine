#include "Group.h"

#include "Frame.h"

using namespace chira;

Group::Group(std::string name_) : Entity(std::move(name_)) {}

Group::Group() : Entity() {}

const Group* Group::getGroup() const {
    return this;
}

Group* Group::getGroup() {
    return this;
}

glm::vec3 Group::getGlobalPosition() const {
    return this->position;
}

