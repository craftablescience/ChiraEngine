#pragma once

#include <deque>
#include <resource/FontResource.h>
#include <utility/Logger.h>
#include <ui/IPanel.h>

namespace chira {

class ConsolePanel : public IPanel {
public:
    explicit ConsolePanel(ImVec2 windowSize = ImVec2{800, 600});
    ~ConsolePanel() override;
    void renderContents() override;
    void clear();
    void addLog(const std::string& message);
    void setTheme();
    void resetTheme() const;

    static constexpr int MAX_ITEM_COUNT = 1024;
private:
    static void processConsoleMessage(std::string_view message);
    SharedPointer<FontResource> font;
    std::deque<std::string> items;
    bool autoScroll;
    uuids::uuid loggingId;
};

} // namespace chira
