#include "discordRichPresence.h"

#include "../core/engine.h"

void discordRichPresence::init(const std::string& appId) {
    if (discordRichPresence::isInitialized) return;
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = discordRichPresence::callbackReady;
    handlers.disconnected = discordRichPresence::callbackDisconnected;
    handlers.errored = discordRichPresence::callbackError;
    Discord_Initialize(appId.c_str(), &handlers, 1, nullptr);
    discordRichPresence::isInitialized = true;
}

bool discordRichPresence::initialized() {
    return discordRichPresence::isInitialized;
}

void discordRichPresence::setState(const std::string& state_) {
    discordRichPresence::state = state_;
    discordRichPresence::isModifiedSinceLastUpdate = true;
}

void discordRichPresence::setDetails(const std::string& details_) {
    discordRichPresence::details = details_;
    discordRichPresence::isModifiedSinceLastUpdate = true;
}

void discordRichPresence::setLargeImage(const std::string& imageKey) {
    discordRichPresence::largeImage = imageKey;
    discordRichPresence::isModifiedSinceLastUpdate = true;
}
void discordRichPresence::setLargeImageText(const std::string& text) {
    discordRichPresence::largeImageText = text;
    discordRichPresence::isModifiedSinceLastUpdate = true;
}

void discordRichPresence::setSmallImage(const std::string& imageKey) {
    discordRichPresence::smallImage = imageKey;
    discordRichPresence::isModifiedSinceLastUpdate = true;
}

void discordRichPresence::setSmallImageText(const std::string& text) {
    discordRichPresence::smallImageText = text;
    discordRichPresence::isModifiedSinceLastUpdate = true;
}

void discordRichPresence::setStartTimestamp(std::int64_t time) {
    discordRichPresence::startTimestamp = time;
    discordRichPresence::isModifiedSinceLastUpdate = true;
}

void discordRichPresence::setEndTimestamp(std::int64_t time) {
    discordRichPresence::endTimestamp = time;
    discordRichPresence::isModifiedSinceLastUpdate = true;
}

void discordRichPresence::updatePresence() {
    if (discordRichPresence::isModifiedSinceLastUpdate) {
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        if (!discordRichPresence::state.empty())
            discordPresence.state = discordRichPresence::state.c_str();
        if (!discordRichPresence::details.empty())
            discordPresence.details = discordRichPresence::details.c_str();
        if (discordRichPresence::startTimestamp >= 0)
            discordPresence.startTimestamp = discordRichPresence::startTimestamp;
        if (discordRichPresence::endTimestamp >= 0)
            discordPresence.endTimestamp = discordRichPresence::endTimestamp;
        if (!discordRichPresence::largeImage.empty())
            discordPresence.largeImageKey = discordRichPresence::largeImage.c_str();
        if (!discordRichPresence::largeImageText.empty())
            discordPresence.largeImageText = discordRichPresence::largeImageText.c_str();
        if (!discordRichPresence::smallImage.empty())
            discordPresence.smallImageKey = discordRichPresence::smallImage.c_str();
        if (!discordRichPresence::smallImageText.empty())
            discordPresence.smallImageText = discordRichPresence::smallImageText.c_str();
        discordPresence.instance = 0;
        Discord_UpdatePresence(&discordPresence);
        discordRichPresence::isModifiedSinceLastUpdate = false;
    }
#ifdef DISCORD_DISABLE_IO_THREAD
    Discord_UpdateConnection();
#endif
    Discord_RunCallbacks();
}

void discordRichPresence::resetPresence() {
    discordRichPresence::state = "";
    discordRichPresence::details = "";
    discordRichPresence::largeImage = "";
    discordRichPresence::largeImageText = "";
    discordRichPresence::smallImage = "";
    discordRichPresence::smallImageText = "";
    discordRichPresence::startTimestamp = -1;
    discordRichPresence::endTimestamp = -1;
    discordRichPresence::isModifiedSinceLastUpdate = false;
    Discord_ClearPresence();
}

void discordRichPresence::shutdown() {
    if (discordRichPresence::isInitialized) {
        Discord_Shutdown();
    }
}

void discordRichPresence::callbackReady(const DiscordUser* connectedUser) {
    chiraLogger::log(INFO, "Discord", "User " + std::string(connectedUser->username) + ":" + connectedUser->discriminator + " connected");
}

void discordRichPresence::callbackDisconnected(int errcode, const char* message) {
    chiraLogger::log(WARN, "Discord", "User disconnected, code " + std::to_string(errcode) + ": " + std::string(message));
}

void discordRichPresence::callbackError(int errcode, const char* message) {
    chiraLogger::log(ERR, "Discord", "Error " + std::to_string(errcode) + ": " + std::string(message));
}
