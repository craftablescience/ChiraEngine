#pragma once

#include <imgui.h>
#include <entity/entity.h>

namespace chira {
    class window : public entity {
    public:
        window(const std::string& title_, bool startVisible, const ImVec2& windowSize = ImVec2(), bool enforceSize = false);
        void render(const glm::mat4& parentTransform) override;
        virtual void preRenderContents() {};
        virtual void renderContents() = 0;
        virtual void postRenderContents() {};
        void setVisible(bool visible);
        [[nodiscard]] bool isVisible() const;
    protected:
        std::string title;
        bool isVisible_;
        ImVec2 nextWindowSize;
        ImGuiCond_ windowSizeCondition;
        ImGuiWindowFlags flags;
    };
}
