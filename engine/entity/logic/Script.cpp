#include "Script.h"

using namespace chira;

Script::Script(std::string name_, const std::string& scriptID)
    : Entity(std::move(name_))
    /* , script(scriptID) */ {}

Script::Script(const std::string& scriptID)
    : Entity()
    /* , script(scriptID) */ {}

void Script::onAddedToTree() {
    // this->script.callFunction<void>("init");
}

void Script::update() {
    // this->script.callFunction<void>("update");
    Entity::update();
}

void Script::render(glm::mat4 parentTransform) {
    // this->script.callFunction<void>("render");
    Entity::render(parentTransform);
}

Script::~Script() {
    // this->script.callFunction<void>("stop");
}
