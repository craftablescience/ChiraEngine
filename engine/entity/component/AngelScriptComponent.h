#pragma once

#include <entt/entt.hpp>
#include <script/AngelScriptHolder.h>

namespace chira {

// todo(as): this thing crashes the engine on exit and i cant figure out why

struct AngelScriptComponent {
public:
    explicit AngelScriptComponent(const std::string& scriptID)
            : script(scriptID) {}

    void update() const {
        this->script.callFunction<void>("update");
    }

    void render() const {
        this->script.callFunction<void>("render");
    }

    static void onConstruct(entt::registry& registry, entt::entity entity) {
        auto& angelScriptComponent = registry.get<AngelScriptComponent>(entity);
        angelScriptComponent.script.callFunction<void>("init");
    }

    static void onDestroy(entt::registry& registry, entt::entity entity) {
        auto& angelScriptComponent = registry.get<AngelScriptComponent>(entity);
        angelScriptComponent.script.callFunction<void>("stop");
    }

private:
    AngelScriptHolder script;
};

} // namespace chira
