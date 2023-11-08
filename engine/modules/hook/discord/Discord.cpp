#include "Discord.h"

#include <cstring>

#include <discord_rpc.h>

#include <core/config/ConEntry.h>
#include <core/debug/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_REGISTER_MODULE(Discord);

CHIRA_CREATE_LOG(DISCORD);

ConVar discord_enabled{"discord_enabled", true, "Allows applications to use Discord rich presence.", CON_FLAG_CACHE};

void DiscordModule::init(std::string_view appId) {
    static bool ran = false;
    if (ran || !discord_enabled.getValue<bool>())
        return;
    ran = true;

    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
#ifdef DEBUG
    handlers.ready = [](const DiscordUser* connectedUser) {
        // Thanks username update
        if (!strcmp(connectedUser->discriminator, "0")) {
            LOG_DISCORD.info("Discord user {} connected", connectedUser->username);
        } else {
            LOG_DISCORD.info("Discord user {}#{} connected", connectedUser->username, connectedUser->discriminator);
        }
    };
    handlers.disconnected = [](int errcode, const char* message) {
        LOG_DISCORD.warning(TRF("debug.discord.user_disconnected", errcode, message));
    };
    handlers.errored = [](int errcode, const char* message) {
        LOG_DISCORD.error(TRF("debug.discord.generic_error", errcode, message));
    };
#endif
    Discord_Initialize(appId.data(), &handlers, 1, nullptr);
}

void DiscordModule::update() {
    if (!this->isInitialized()) {
        return;
    }
    if (this->dirty && discord_enabled.getValue<bool>()) {
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        if (!this->state.empty())
            discordPresence.state = this->state.c_str();
        if (!this->details.empty())
            discordPresence.details = this->details.c_str();
        if (this->startTimestamp >= 0)
            discordPresence.startTimestamp = this->startTimestamp;
        if (this->endTimestamp >= 0)
            discordPresence.endTimestamp = this->endTimestamp;
        if (!this->largeImage.empty())
            discordPresence.largeImageKey = this->largeImage.c_str();
        if (!this->largeImageText.empty())
            discordPresence.largeImageText = this->largeImageText.c_str();
        if (!this->smallImage.empty())
            discordPresence.smallImageKey = this->smallImage.c_str();
        if (!this->smallImageText.empty())
            discordPresence.smallImageText = this->smallImageText.c_str();

        DiscordButton buttons[2] {{}, {}};
        if (!this->button1.url.empty()) {
            buttons[0].label = this->button1.name.c_str();
            buttons[0].url = this->button1.url.c_str();
        }
        if (!this->button2.url.empty()) {
            buttons[1].label = this->button2.name.c_str();
            buttons[1].url = this->button2.url.c_str();
        }
        discordPresence.buttons = buttons;

        discordPresence.instance = 0;
        Discord_UpdatePresence(&discordPresence);
        this->dirty = false;
    }
#ifdef DISCORD_DISABLE_IO_THREAD
    Discord_UpdateConnection();
#endif
    Discord_RunCallbacks();
}

void DiscordModule::deinit() {
    if (this->isInitialized()) {
        Discord_Shutdown();
    }
}

void DiscordModule::setState(const std::string& state_) {
    chiraAssert(state_.length() < 128, "Discord state length limit exceeded! Max 127 characters");
    this->state = state_;
    this->dirty = true;
}

void DiscordModule::setDetails(const std::string& details_) {
    chiraAssert(details_.length() < 128, "Discord details length limit exceeded! Max 127 characters");
    this->details = details_;
    this->dirty = true;
}

void DiscordModule::setLargeImage(const std::string& imageKey) {
    chiraAssert(imageKey.length() < 32, "Discord large image length limit exceeded! Max 31 characters");
    this->largeImage = imageKey;
    this->dirty = true;
}
void DiscordModule::setLargeImageText(const std::string& text) {
    chiraAssert(text.length() < 128, "Discord large image text length limit exceeded! Max 127 characters");
    this->largeImageText = text;
    this->dirty = true;
}

void DiscordModule::setSmallImage(const std::string& imageKey) {
    chiraAssert(imageKey.length() < 32, "Discord small image length limit exceeded! Max 31 characters");
    this->smallImage = imageKey;
    this->dirty = true;
}

void DiscordModule::setSmallImageText(const std::string& text) {
    chiraAssert(text.length() < 128, "Discord small image text length limit exceeded! Max 127 characters");
    this->smallImageText = text;
    this->dirty = true;
}

void DiscordModule::setStartTimestamp(std::int64_t time) {
    this->startTimestamp = time;
    this->dirty = true;
}

void DiscordModule::setEndTimestamp(std::int64_t time) {
    this->endTimestamp = time;
    this->dirty = true;
}

void DiscordModule::setTopButton(const DiscordButtonData& button) {
    this->button1 = button;
    this->dirty = true;
}

void DiscordModule::setBottomButton(const DiscordButtonData& button) {
    this->button2 = button;
    this->dirty = true;
}

void DiscordModule::resetPresence() {
    this->state = "";
    this->details = "";
    this->largeImage = "";
    this->largeImageText = "";
    this->smallImage = "";
    this->smallImageText = "";
    this->startTimestamp = -1;
    this->endTimestamp = -1;
    this->button1.name = "";
    this->button1.url = "";
    this->button2.name = "";
    this->button2.url = "";
    Discord_ClearPresence();
    this->dirty = false;
}
