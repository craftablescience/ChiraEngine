#pragma once

#include <string>
#include <discord_rpc.h>

namespace chira {
    /// Updates are handled by the engine. All you need to do is run the init() method.
    /// After that, any setter functions will change the status after the frame is rendered.
    class discordRichPresence {
    public:
        static void init(const std::string& appId);
        static bool initialized();
        static void setState(const std::string& state_);
        static void setDetails(const std::string& details_);
        static void setLargeImage(const std::string& imageKey);
        static void setLargeImageText(const std::string& text);
        static void setSmallImage(const std::string& imageKey);
        static void setSmallImageText(const std::string& text);
        static void setStartTimestamp(std::int64_t time);
        static void setEndTimestamp(std::int64_t time);
        static void updatePresence();
        static void resetPresence();
        static void shutdown();
    private:
        static inline bool isInitialized = false;
        static inline bool isModifiedSinceLastUpdate = false;
        static inline std::string state; // NOLINT(cert-err58-cpp)
        static inline std::string details; // NOLINT(cert-err58-cpp)
        static inline std::string largeImage; // NOLINT(cert-err58-cpp)
        static inline std::string largeImageText; // NOLINT(cert-err58-cpp)
        static inline std::string smallImage; // NOLINT(cert-err58-cpp)
        static inline std::string smallImageText; // NOLINT(cert-err58-cpp)
        static inline std::int64_t startTimestamp = -1;
        static inline std::int64_t endTimestamp = -1;
        static void callbackReady(const DiscordUser* connectedUser);
        static void callbackDisconnected(int errcode, const char* message);
        static void callbackError(int errcode, const char* message);
    };
}
