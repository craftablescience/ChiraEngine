#pragma once

#include <string>
#include <imgui.h>

namespace chira {
    class abstractUiWindowComponent {
    public:
        abstractUiWindowComponent(const std::string& title_, bool startVisible, const ImVec2& windowSize = ImVec2(0,0), bool enforceSize = false);
        void render(double delta);
        virtual void draw(double delta) = 0;
        void setVisible(bool visible);
        [[nodiscard]] bool isVisible() const;
    private:
        std::string title;
        bool isVisible_;
        ImVec2 nextWindowSize;
        ImGuiCond_ windowSizeCondition;
    };
}
