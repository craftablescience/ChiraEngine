#include "DiscordRPC.h"

#include <discord_rpc.h>
#include <utility/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

bool DiscordRPC::isInitialized = false;
bool DiscordRPC::isModifiedSinceLastUpdate = false;
std::string DiscordRPC::state;
std::string DiscordRPC::details;
std::string DiscordRPC::largeImage;
std::string DiscordRPC::largeImageText;
std::string DiscordRPC::smallImage;
std::string DiscordRPC::smallImageText;
std::int64_t DiscordRPC::startTimestamp = -1;
std::int64_t DiscordRPC::endTimestamp = -1;

void DiscordRPC::init(std::string_view appId) {
    if (DiscordRPC::isInitialized)
        return;

    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
#if DEBUG
    handlers.ready = [](const DiscordUser* connectedUser) {
        Logger::log(LOG_INFO_IMPORTANT, "Discord", TRF("debug.discord.user_connected", connectedUser->username, connectedUser->discriminator));
    };
    handlers.disconnected = [](int errcode, const char* message) {
        Logger::log(LOG_WARNING, "Discord", TRF("debug.discord.user_disconnected", errcode, message));
    };
    handlers.errored = [](int errcode, const char* message) {
        Logger::log(LOG_ERROR, "Discord", TRF("debug.discord.generic_error", errcode, message));
    };
#endif
    Discord_Initialize(appId.data(), &handlers, 1, nullptr);
    DiscordRPC::isInitialized = true;
}

bool DiscordRPC::initialized() {
    return DiscordRPC::isInitialized;
}

void DiscordRPC::setState(const std::string& state_) {
    DiscordRPC::state = state_;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::setDetails(const std::string& details_) {
    DiscordRPC::details = details_;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::setLargeImage(const std::string& imageKey) {
    DiscordRPC::largeImage = imageKey;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}
void DiscordRPC::setLargeImageText(const std::string& text) {
    DiscordRPC::largeImageText = text;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::setSmallImage(const std::string& imageKey) {
    DiscordRPC::smallImage = imageKey;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::setSmallImageText(const std::string& text) {
    DiscordRPC::smallImageText = text;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::setStartTimestamp(std::int64_t time) {
    DiscordRPC::startTimestamp = time;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::setEndTimestamp(std::int64_t time) {
    DiscordRPC::endTimestamp = time;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::updatePresence() {
    if (DiscordRPC::isModifiedSinceLastUpdate) {
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        if (!DiscordRPC::state.empty())
            discordPresence.state = DiscordRPC::state.c_str();
        if (!DiscordRPC::details.empty())
            discordPresence.details = DiscordRPC::details.c_str();
        if (DiscordRPC::startTimestamp >= 0)
            discordPresence.startTimestamp = DiscordRPC::startTimestamp;
        if (DiscordRPC::endTimestamp >= 0)
            discordPresence.endTimestamp = DiscordRPC::endTimestamp;
        if (!DiscordRPC::largeImage.empty())
            discordPresence.largeImageKey = DiscordRPC::largeImage.c_str();
        if (!DiscordRPC::largeImageText.empty())
            discordPresence.largeImageText = DiscordRPC::largeImageText.c_str();
        if (!DiscordRPC::smallImage.empty())
            discordPresence.smallImageKey = DiscordRPC::smallImage.c_str();
        if (!DiscordRPC::smallImageText.empty())
            discordPresence.smallImageText = DiscordRPC::smallImageText.c_str();
        discordPresence.instance = 0;
        Discord_UpdatePresence(&discordPresence);
        DiscordRPC::isModifiedSinceLastUpdate = false;
    }
#ifdef DISCORD_DISABLE_IO_THREAD
    Discord_UpdateConnection();
#endif
    Discord_RunCallbacks();
}

void DiscordRPC::resetPresence() {
    DiscordRPC::state = "";
    DiscordRPC::details = "";
    DiscordRPC::largeImage = "";
    DiscordRPC::largeImageText = "";
    DiscordRPC::smallImage = "";
    DiscordRPC::smallImageText = "";
    DiscordRPC::startTimestamp = -1;
    DiscordRPC::endTimestamp = -1;
    DiscordRPC::isModifiedSinceLastUpdate = false;
    Discord_ClearPresence();
}

void DiscordRPC::shutdown() {
    if (DiscordRPC::isInitialized) {
        Discord_Shutdown();
    }
}
