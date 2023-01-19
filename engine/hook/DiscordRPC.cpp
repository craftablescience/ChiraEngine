#include "DiscordRPC.h"

#include <discord_rpc.h>
#include <config/ConEntry.h>
#include <core/Logger.h>
#include <core/System.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(DISCORD);

ConVar discord_enable{"discord_enable", true, "Allows applications to use Discord rich presence.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

CHIRA_CREATE_SYSTEM(Discord) {
    // Discord should be initialized manually before Engine::init
    static void update() {
        if (DiscordRPC::initialized()) {
            DiscordRPC::updatePresence();
        }
    }
    static void deinit() {
        if (DiscordRPC::initialized()) {
            DiscordRPC::shutdown();
        }
    }
};

void DiscordRPC::init(std::string_view appId) {
    if (DiscordRPC::isInitialized || !discord_enable.getValue<bool>())
        return;

    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
#if DEBUG
    handlers.ready = [](const DiscordUser* connectedUser) {
        LOG_DISCORD.info(TRF("debug.discord.user_connected", connectedUser->username, connectedUser->discriminator));
    };
    handlers.disconnected = [](int errcode, const char* message) {
        LOG_DISCORD.warning(TRF("debug.discord.user_disconnected", errcode, message));
    };
    handlers.errored = [](int errcode, const char* message) {
        LOG_DISCORD.error(TRF("debug.discord.generic_error", errcode, message));
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

void DiscordRPC::setTopButton(const DiscordButtonData& button) {
    DiscordRPC::button1 = button;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::setBottomButton(const DiscordButtonData& button) {
    DiscordRPC::button2 = button;
    DiscordRPC::isModifiedSinceLastUpdate = true;
}

void DiscordRPC::updatePresence() {
    if (DiscordRPC::isModifiedSinceLastUpdate && discord_enable.getValue<bool>()) {
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

        DiscordButton buttons[2] {{}, {}};
        if (!DiscordRPC::button1.url.empty()) {
            buttons[0].label = DiscordRPC::button1.name.c_str();
            buttons[0].url = DiscordRPC::button1.url.c_str();
        }
        if (!DiscordRPC::button2.url.empty()) {
            buttons[1].label = DiscordRPC::button2.name.c_str();
            buttons[1].url = DiscordRPC::button2.url.c_str();
        }
        discordPresence.buttons = buttons;

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
    DiscordRPC::button1.name = "";
    DiscordRPC::button1.url = "";
    DiscordRPC::button2.name = "";
    DiscordRPC::button2.url = "";
    DiscordRPC::isModifiedSinceLastUpdate = false;
    Discord_ClearPresence();
}

void DiscordRPC::shutdown() {
    if (DiscordRPC::isInitialized) {
        Discord_Shutdown();
    }
}
