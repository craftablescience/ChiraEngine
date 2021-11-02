#include "entity.h"

#include "../utility/uuidGenerator.h"
#include "../utility/string/stringRemove.h"

using namespace chira;

entity::entity(entity* parent_) {
    this->parent = parent_;
    this->name = uuidGenerator::getNewUUIDString();
}

entity::entity(entity* parent_, const std::string& name_) {
    this->parent = parent_;
    this->name = name_;
    replace(this->name, "/");
}

void entity::render(const glm::mat4& parentTransform) {
    for (auto& [key, entity] : this->children) {
        entity->render(parentTransform);
    }
}

entity* entity::getParent() const {
    return this->parent;
}

std::string_view entity::getName() const {
    return this->name;
}

entity* entity::getChild(const std::string& name_) {
    return this->children[name_];
}
