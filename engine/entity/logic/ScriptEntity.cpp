#include "ScriptEntity.h"

using namespace chira;

ScriptEntity::ScriptEntity(std::string name_, const std::string& scriptID)
    : Entity(std::move(name_))
    , script(scriptID) {
    this->script.getFunction("init")();
    this->updateFunc = this->script.getFunction("update");
    this->renderFunc = this->script.getFunction("render");
}

ScriptEntity::ScriptEntity(const std::string& scriptID)
    : Entity()
    , script(scriptID) {
    this->script.getFunction("init")();
    this->updateFunc = this->script.getFunction("update");
    this->renderFunc = this->script.getFunction("render");
}

void ScriptEntity::update() {
    this->updateFunc();
    Entity::update();
}

void ScriptEntity::render(glm::mat4 parentTransform) {
    this->renderFunc();
    Entity::render(parentTransform);
}

ScriptEntity::~ScriptEntity() {
    this->script.getFunction("stop")();
}
