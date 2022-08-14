#include "Script.h"

using namespace chira;

Script::Script(std::string name_, const std::string& scriptID)
    : Entity(std::move(name_))
    , script(scriptID) {
    this->script.getFunction("init")();
    this->updateFunc = this->script.getFunction("update");
    this->renderFunc = this->script.getFunction("render");
}

Script::Script(const std::string& scriptID)
    : Entity()
    , script(scriptID) {
    this->script.getFunction("init")();
    this->updateFunc = this->script.getFunction("update");
    this->renderFunc = this->script.getFunction("render");
}

void Script::update() {
    this->updateFunc();
    Entity::update();
}

void Script::render(glm::mat4 parentTransform) {
    this->renderFunc();
    Entity::render(parentTransform);
}

Script::~Script() {
    this->script.getFunction("stop")();
}