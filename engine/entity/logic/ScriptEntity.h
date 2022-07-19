#pragma once

#include <entity/Entity.h>
#include <script/AngelScriptHolder.h>

namespace chira {

class ScriptEntity : public Entity {
public:
    ScriptEntity(std::string name_, const std::string& scriptID);
    explicit ScriptEntity(const std::string& scriptID);
    void update() override;
    void render(glm::mat4 parentTransform) override;
    ~ScriptEntity() override;
private:
    AngelScriptHolder script;
    std::function<void()> updateFunc;
    std::function<void()> renderFunc;
};

}
