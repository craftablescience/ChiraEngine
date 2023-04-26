#pragma once

#include <entt/entt.hpp>
#include <script/AngelScriptHolder.h>

namespace chira {

struct AngelScriptComponent {
    static constexpr auto in_place_delete = true;

    explicit AngelScriptComponent(const std::string& scriptID = "file://scripts/empty.as")
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
        angelScriptComponent.script.callFunction<void>("deinit");
    }

    AngelScriptHolder* getScript() {
        return &this->script;
    }

private:
    AngelScriptHolder script;
};

} // namespace chira
