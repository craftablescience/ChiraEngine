#pragma once

#include <deque>

#include <core/debug/Logger.h>
#include "../Font.h"
#include "../IPanel.h"

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

    static constexpr int MAX_ITEM_COUNT = 512;
private:
    void processConsoleMessage(std::string_view message);
    SharedPointer<Font> font;
    std::deque<std::string> items;
    bool autoScroll;
    uuids::uuid loggingId;
};

} // namespace chira
