#pragma once

#include <string>
#include <string_view>
#include <imgui.h>
#include <math/Types.h>
#include <entity/Entity.h>
#include <entity/camera/EditorCamera.h>

namespace chira {

class IPanel {
public:
    IPanel(std::string title_, bool startVisible, ImVec2 windowSize = {}, bool enforceSize = false);
    virtual ~IPanel() = default;
    void render();
    virtual void preRenderContents() {};
    virtual void renderContents() = 0;
    virtual void postRenderContents() {};
    [[nodiscard]] std::string_view getTitle() const;
    void setTitle(const std::string& newTitle);
    [[nodiscard]] bool isVisible() const;
    void setVisible(bool visible_);
    [[nodiscard]] bool wasActivatedThisFrame() const;
    [[nodiscard]] glm::vec2 getWindowSize() const;
    void setWindowSize(glm::vec2 size);
    ImGuiWindowFlags& getWindowFlags();
    std::string_view addChild(chira::Entity *child);
    void removeChild(std::string_view name_);
    bool hasChild(std::string_view name_);
    Entity* getChild(std::string_view name_);
    void setCamera(Camera* camera);
protected:
    std::string title;
    bool visible;
    bool activatedThisFrame;
    ImVec2 nextWindowSize;
    ImGuiCond_ windowSizeCondition;
    ImGuiWindowFlags flags;
};

} // namespace chira
