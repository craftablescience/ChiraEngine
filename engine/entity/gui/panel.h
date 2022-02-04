#pragma once

#include <imgui.h>
#include <entity/entity.h>

namespace chira {
    class Panel : public Entity {
    public:
        Panel(const std::string& title_, bool startVisible, ImVec2 windowSize = {}, bool enforceSize = false);
        void render(glm::mat4 parentTransform) override;
        virtual void preRenderContents() {};
        virtual void renderContents() = 0;
        virtual void postRenderContents() {};
    protected:
        std::string title;
        ImVec2 nextWindowSize;
        ImGuiCond_ windowSizeCondition;
        ImGuiWindowFlags flags;
    };
}
