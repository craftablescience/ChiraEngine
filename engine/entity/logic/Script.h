#pragma once

#include <entity/Entity.h>
#include <script/AngelScriptHolder.h>

namespace chira {

class Script : public Entity {
public:
    Script(std::string name_, const std::string& scriptID);
    explicit Script(const std::string& scriptID);
    void onAddedToTree() override;
    void update() override;
    void render(glm::mat4 parentTransform) override;
    ~Script() override;
private:
    AngelScriptHolder script;
};

} // namespace chira
