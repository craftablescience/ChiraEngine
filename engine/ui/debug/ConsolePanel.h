#pragma once

#include <resource/FontResource.h>
#include <utility/Logger.h>
#include <ui/IPanel.h>

namespace chira {

class ConsolePanel : public IPanel {
public:
    explicit ConsolePanel(ImVec2 windowSize = ImVec2{800, 600});
    ~ConsolePanel() override;
    void renderContents() override;
    void clearLog();
    void addLog(const std::string& message);
    void setTheme();
    void resetTheme() const;
private:
    SharedPointer<FontResource> font;
    ImVector<char*> items;
    ImVector<char*> history;
    bool autoScroll;
    uuids::uuid loggingId;
};

} // namespace chira
