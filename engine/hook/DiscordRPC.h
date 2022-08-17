#pragma once

#include <cstdint>
#include <string>
#include <string_view>

struct DiscordUser;

namespace chira {

struct DiscordButtonData {
    std::string name;
    std::string url;
};

/// Updates are handled by the engine. All you need to do is run the init() method.
/// After that, any setter functions will change the status after update().
class DiscordRPC {
public:
    DiscordRPC() = delete;
    static void init(std::string_view appId);
    static bool initialized();
    static void setState(const std::string& state_);
    static void setDetails(const std::string& details_);
    static void setLargeImage(const std::string& imageKey);
    static void setLargeImageText(const std::string& text);
    static void setSmallImage(const std::string& imageKey);
    static void setSmallImageText(const std::string& text);
    static void setStartTimestamp(std::int64_t time);
    static void setEndTimestamp(std::int64_t time);
    static void setTopButton(const DiscordButtonData& button);
    static void setBottomButton(const DiscordButtonData& button);
    static void updatePresence();
    static void resetPresence();
    static void shutdown();
private:
    static inline bool isInitialized = false;
    static inline bool isModifiedSinceLastUpdate = false;
    static inline std::string state;
    static inline std::string details;
    static inline std::string largeImage;
    static inline std::string largeImageText;
    static inline std::string smallImage;
    static inline std::string smallImageText;
    static inline std::int64_t startTimestamp = -1;
    static inline std::int64_t endTimestamp = -1;
    static inline DiscordButtonData button1, button2;
};

} // namespace chira
