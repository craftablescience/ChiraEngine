#include "Group.h"

#include <core/Engine.h>
#include <render/shader/UBO.h>
#include "Frame.h"

using namespace chira;

Group::Group(std::string name_) : Entity(std::move(name_)) {}

Group::Group() : Entity() {}

glm::vec3 Group::getGlobalPosition() {
    return this->position;
}

const Group* Group::getGroup() const {
    return this;
}

Group* Group::getGroup() {
    return this;
}
