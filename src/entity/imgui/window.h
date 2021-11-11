#pragma once

#include <functional>
#include <string>
#include <imgui.h>
#include "../entity.h"

namespace chira {
    class window : public entity {
    public:
        window(const std::string& title_, bool startVisible, const ImVec2& windowSize = ImVec2(0,0), bool enforceSize = false, const std::function<void()>& windowFunc = [](){});
        void render(const glm::mat4& parentTransform) override;
        virtual void renderContents();
        void setVisible(bool visible);
        [[nodiscard]] bool isVisible() const;
    protected:
        std::string title;
        bool isVisible_;
        ImVec2 nextWindowSize;
        ImGuiCond_ windowSizeCondition;
        std::function<void()> windowFunction;
    };
}
