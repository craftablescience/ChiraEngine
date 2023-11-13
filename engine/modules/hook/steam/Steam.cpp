#include "Steam.h"

#include <fstream>

#include <libloader.hpp>

#include <core/config/ConEntry.h>
#include <core/debug/Logger.h>
#include <core/utility/String.h>
#include <core/IModule.h>

using namespace chira;
using namespace libloader;

CHIRA_CREATE_LOG(STEAM);

ConVar steam_enabled{"steam_enabled", true, "Initialize Steam API functions.", CON_FLAG_CACHE};

CHIRA_CREATE_MODULE(Steam) {
    CHIRA_CREATE_MODULE_DEPS();

    bool preinit() override {
        if (!steam_enabled.getValue<bool>() || !Steam::Client::initSteam()) {
            LOG_STEAM.warning("Steam failed to initialize!");
	        return IModule::preinit();
        }
	    this->steamStarted = true;
        return IModule::preinit();
    }

    void update() override {
        if (this->steamStarted) {
            Steam::Client::runCallbacks();
        }
    }

    void deinit() override {
        if (this->steamStarted) {
            Steam::Client::shutdown();
        }
    }

private:
	bool steamStarted = false;
};
CHIRA_REGISTER_MODULE(Steam);

/// Helper function to stop repeating stuff
template<typename T, typename U, typename... Params>
inline T steamFunctionWrapper(const std::string& function, T defaultValue, U steamSingleton, Params... params) {
    if (steamSingleton) {
        auto out = Steam::get().call<T>(function, steamSingleton, std::forward<Params>(params)...);
        if (!out) {
            return defaultValue;
        }
        return *out;
    }
    return defaultValue;
}

/// Helper function to stop repeating stuff
template<typename T, typename... Params>
inline std::string steamFunctionStringWrapper(const std::string& function, const std::string& defaultValue, T steamSingleton, Params... params) {
    if (steamSingleton) {
        auto out = Steam::get().call<const char*>(function, steamSingleton, std::forward<Params>(params)...);
        return out ? std::string{*out} : defaultValue;
    }
    return defaultValue;
}

const library& Steam::get() {
    static library steamBinary{"steam_api" + std::to_string(ENVIRONMENT_TYPE)};
    return steamBinary;
}

void Steam::generateAppIDFile(unsigned int appID) {
    std::ofstream file{"steam_appid.txt"};
    file << std::to_string(appID) << std::endl;
    file.close();
}

void* Steam::Client::get() {
    if (auto client = Steam::get().call<void*>("SteamClient"))
        return *client;
    return nullptr;
}

bool Steam::Client::initSteam() {
    static bool initialized = false;
    if (initialized)
        return true;
    if (auto out = Steam::get().call<bool>("SteamAPI_Init"); out && *out) {
        initialized = true;
        // Handle callbacks manually
        Steam::get().call<void>("SteamAPI_ManualDispatch_Init");
        return true;
    }
    return false;
}

void Steam::Client::runCallbacks() {
    std::int32_t steamPipe;
    if (auto client = Steam::Client::get())
        steamPipe = steamFunctionWrapper<std::int32_t>("SteamAPI_GetHSteamPipe", 0, client);
    else
        return;
    Steam::get().call<void>("SteamAPI_ManualDispatch_RunFrame", steamPipe);
    CallbackMessage callback{};
    while (steamFunctionWrapper<bool>("SteamAPI_ManualDispatch_GetNextCallback", false, steamPipe, &callback)) {
        switch (static_cast<CallbackMessageType>(callback.callbackType)) {
            using enum CallbackMessageType;
            case GAME_OVERLAY_ACTIVATED:
                // todo(events): add game overlay enable/disable event
                //Events::createEvent("chira::steam::game_overlay_activated", static_cast<bool>(reinterpret_cast<Callbacks::GameOverlayActivated*>(callback.callback)->active));
                break;
            case COMPLETED: {
                void* callbackResult = std::malloc(callback.callbackSize);
                if (bool failed = false; steamFunctionWrapper<bool>(
                        "SteamAPI_ManualDispatch_GetAPICallResult",
                        false,
                        steamPipe,
                        reinterpret_cast<Callbacks::Completed*>(callback.callback)->asyncCallbackId,
                        callbackResult,
                        callback.callback,
                        callback.callbackType,
                        &failed
                    )) {
                    // todo: dispatch callbacks that go through this thing
                    // Dispatch the call result to the registered handler(s) for the
                    // call identified by pCallCompleted->m_hAsyncCall
                }
                std::free(callbackResult);
                break;
            }
            case DLC_INSTALLED:
                // todo(events): add dlc installed event
                //Events::createEvent("chira::steam::dlc_installed", static_cast<std::uint32_t>(reinterpret_cast<Callbacks::DLCInstalled*>(callback.callback)->appID));
                break;
            case FILE_DETAILS_RESULT:
                // todo(events): add file details callback event
                //Events::createEvent("chira::steam::file_details_result", *reinterpret_cast<Callbacks::FileDetailsResult*>(callback.callback));
                break;
        }
        Steam::get().call<void>("SteamAPI_ManualDispatch_FreeLastCallback", steamPipe);
    }
    // Not called automatically anymore!
    Steam::get().call<void>("SteamAPI_ReleaseCurrentThreadMemory");
}

void Steam::Client::shutdown() {
    Steam::get().call<void>("SteamAPI_Shutdown");
}

// -------------------------------- USER -------------------------------- //

void* Steam::User::get() {
    if (auto user = Steam::get().call<void*>("SteamAPI_SteamUser_v021"))
        return *user;
    return nullptr;
}

bool Steam::User::isLoggedOn() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUser_BLoggedOn", false, Steam::User::get());
}

std::uint64_t Steam::User::getSteamID() {
    return steamFunctionWrapper<std::uint64_t>("SteamAPI_ISteamUser_GetSteamID", 0, Steam::User::get());
}

bool Steam::User::isBehindNAT() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUser_BIsBehindNAT", false, Steam::User::get());
}

int Steam::User::getGameBadgeLevel(bool foil, int series) {
    return steamFunctionWrapper<int>("SteamAPI_ISteamUser_GetGameBadgeLevel", 0, Steam::User::get(), series, foil);
}

int Steam::User::getPlayerSteamLevel() {
    return steamFunctionWrapper<int>("SteamAPI_ISteamUser_GetPlayerSteamLevel", -1, Steam::User::get());
}

bool Steam::User::isPhoneVerified() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUser_BIsPhoneVerified", false, Steam::User::get());
}

bool Steam::User::isTwoFactorAuthenticationEnabled() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUser_BIsTwoFactorEnabled", false, Steam::User::get());
}

bool Steam::User::isPhoneIdentifying() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUser_BIsPhoneIdentifying", false, Steam::User::get());
}

bool Steam::User::isPhoneRequiringVerification() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUser_BIsPhoneRequiringVerification", false, Steam::User::get());
}

std::uint64_t Steam::User::getMarketEligibility() {
    return steamFunctionWrapper<std::uint64_t>("SteamAPI_ISteamUser_GetMarketEligibility", 0, Steam::User::get());
}

// -------------------------------- FRIENDS -------------------------------- //

void* Steam::Friends::get() {
    if (auto friends = Steam::get().call<void*>("SteamAPI_SteamFriends_v017"))
        return *friends;
    return nullptr;
}

std::string Steam::Friends::getPersonaName() {
    return steamFunctionStringWrapper("SteamAPI_ISteamFriends_GetPersonaName", "", Steam::Friends::get());
}

std::uint64_t Steam::Friends::setPersonaName(std::string_view name) {
    return steamFunctionWrapper<std::uint64_t>("SteamAPI_ISteamFriends_SetPersonaName", 0, Steam::Friends::get(), name.data());
}

// -------------------------------- UTILS -------------------------------- //

void* Steam::Utils::get() {
    if (auto utils = Steam::get().call<void*>("SteamAPI_SteamUtils_v010"))
        return *utils;
    return nullptr;
}

std::uint32_t Steam::Utils::getSecondsSinceAppActive() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetSecondsSinceAppActive", 0, Steam::Utils::get());
}

std::uint32_t Steam::Utils::getSecondsSinceComputerActive() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetSecondsSinceComputerActive", 0, Steam::Utils::get());
}

std::uint32_t Steam::Utils::getServerRealTime() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetServerRealTime", 0, Steam::Utils::get());
}

std::string Steam::Utils::getIPCountry() {
    return steamFunctionStringWrapper<bool>("SteamAPI_ISteamUtils_GetIPCountry", "", Steam::Utils::get());
}

bool Steam::Utils::getImageSize(int imageID, std::uint32_t* width, std::uint32_t* height) {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUtils_GetImageSize", false, Steam::Utils::get(), imageID, width, height);
}

bool Steam::Utils::getImageRGBA(int imageID, std::uint8_t* imageBuffer, int imageBufferSize) {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUtils_GetImageRGBA", false, Steam::Utils::get(), imageID, imageBuffer, imageBufferSize);
}

std::uint8_t Steam::Utils::getCurrentBatteryPower() {
    return steamFunctionWrapper<std::uint8_t>("SteamAPI_ISteamUtils_GetCurrentBatteryPower", 0, Steam::Utils::get());
}

std::uint32_t Steam::Utils::getAppID() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetAppID", 0, Steam::Utils::get());
}

void Steam::Utils::setOverlayNotificationPosition(NotificationPosition position) {
    if (auto utils = Steam::Utils::get())
        Steam::get().call<void>("SteamAPI_ISteamUtils_SetOverlayNotificationPosition", utils, position);
}

std::uint32_t Steam::Utils::getIPCCallCount() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetIPCCallCount", 0, Steam::Utils::get());
}

bool Steam::Utils::isOverlayEnabled() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUtils_IsOverlayEnabled", false, Steam::Utils::get());
}

bool Steam::Utils::isRunningInVR() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUtils_IsSteamRunningInVR", false, Steam::Utils::get());
}

void Steam::Utils::setOverlayNotificationInset(int horizontalInset, int verticalInset) {
    if (auto utils = Steam::Utils::get())
        Steam::get().call<void>("SteamAPI_ISteamUtils_SetOverlayNotificationInset", utils, horizontalInset, verticalInset);
}

bool Steam::Utils::isBigPictureModeOn() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUtils_IsSteamInBigPictureMode", false, Steam::Utils::get());
}

void Steam::Utils::startVRDashboard() {
    if (auto utils = Steam::Utils::get())
        Steam::get().call<void>("SteamAPI_ISteamUtils_StartVRDashboard", utils);
}

bool Steam::Utils::isVRHeadsetStreamingEnabled() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUtils_IsVRHeadsetStreamingEnabled", false, Steam::Utils::get());
}

void Steam::Utils::setVRHeadsetStreamingEnabled(bool enabled) {
    if (auto utils = Steam::Utils::get())
        Steam::get().call<void>("SteamAPI_ISteamUtils_SetVRHeadsetStreamingEnabled", utils, enabled);
}

bool Steam::Utils::isRunningOnSteamDeck() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamUtils_IsSteamRunningOnSteamDeck", false, Steam::Utils::get());
}

void Steam::Utils::setGameLauncherMode(bool launcherMode) {
    if (auto utils = Steam::Utils::get())
        Steam::get().call<void>("SteamAPI_ISteamUtils_SetGameLauncherMode", utils, launcherMode);
}

// -------------------------------- USER STATS -------------------------------- //

void* Steam::UserStats::get() {
    if (auto userStats = Steam::get().call<void*>("SteamAPI_SteamUserStats_v012"))
        return *userStats;
    return nullptr;
}

// -------------------------------- APPS -------------------------------- //

void* Steam::Apps::get() {
    if (auto apps = Steam::get().call<void*>("SteamAPI_SteamApps_v008"))
        return *apps;
    return nullptr;
}

bool Steam::Apps::userOwnsThisAppID() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsSubscribed", false, Steam::Apps::get());
}

bool Steam::Apps::isLowViolence() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsLowViolence", false, Steam::Apps::get());
}

bool Steam::Apps::isCybercafe() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsCybercafe", false, Steam::Apps::get());
}

bool Steam::Apps::isVACBanned() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsVACBanned", false, Steam::Apps::get());
}

std::string Steam::Apps::getCurrentGameLanguage() {
    return steamFunctionStringWrapper("SteamAPI_ISteamApps_GetCurrentGameLanguage", "", Steam::Apps::get());
}

std::vector<std::string> Steam::Apps::getAvailableGameLanguages() {
    return String::split(steamFunctionStringWrapper("SteamAPI_ISteamApps_GetAvailableGameLanguages", "", Steam::Apps::get()), ',');
}

bool Steam::Apps::isSubscribedApp(std::uint32_t appID) {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsSubscribedApp", false, Steam::Apps::get(), appID);
}

bool Steam::Apps::isDLCInstalled(std::uint32_t appID) {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsDlcInstalled", false, Steam::Apps::get(), appID);
}

std::uint32_t Steam::Apps::getEarliestPurchaseUnixTime(std::uint32_t appID) {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamApps_GetEarliestPurchaseUnixTime", 0, Steam::Apps::get(), appID);
}

bool Steam::Apps::isSubscribedFromFreeWeekend() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsSubscribedFromFreeWeekend", false, Steam::Apps::get());
}

int Steam::Apps::getDLCCount() {
    return steamFunctionWrapper<int>("SteamAPI_ISteamApps_GetDLCCount", 0, Steam::Apps::get());
}

bool Steam::Apps::getDLCData(int dlc, std::uint32_t* appID, bool* available, std::string& name) {
    auto apps = Steam::Apps::get();
    if (!apps) {
        return false;
    }
    char cname[MAX_API_STRING_LENGTH];
    auto isValid = Steam::get().call<bool>("SteamAPI_ISteamApps_BGetDLCDataByIndex", apps, dlc, appID, available, cname, MAX_API_STRING_LENGTH);
    if (!isValid || !(*isValid)) {
        return false;
    }
    name = std::string{cname};
    return true;
}

void Steam::Apps::installDLC(std::uint32_t appID) {
    if (auto apps = Steam::Apps::get())
        Steam::get().call<void>("SteamAPI_ISteamApps_InstallDLC", apps, appID);
}

void Steam::Apps::uninstallDLC(std::uint32_t appID) {
    if (auto apps = Steam::Apps::get())
        Steam::get().call<void>("SteamAPI_ISteamApps_UninstallDLC", apps, appID);
}

std::string Steam::Apps::getCurrentBranch() {
    auto apps = Steam::Apps::get();
    if (!apps) {
        return "";
    }
    char out[MAX_API_STRING_LENGTH];
    auto isOnBeta = Steam::get().call<bool>("SteamAPI_ISteamApps_GetCurrentBetaName", apps, out, MAX_API_STRING_LENGTH);
    if (!isOnBeta || !(*isOnBeta)) {
        return "";
    }
    std::string path{out};
    return path;
}

bool Steam::Apps::markContentCorrupt(bool missingFilesOnly) {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_MarkContentCorrupt", false, Steam::Apps::get(), missingFilesOnly);
}

std::vector<std::uint32_t> Steam::Apps::getInstalledDepots(std::uint32_t appID) {
    auto apps = Steam::Apps::get();
    if (!apps) {
        return {};
    }
    auto* out = new std::uint32_t[32]; // GodotSteam uses 32 so we do too
    auto size = Steam::get().call<std::uint32_t>("SteamAPI_ISteamApps_GetInstalledDepots", apps, appID, out, 32);
    if (!size) {
        return {};
    }
    std::vector<std::uint32_t> depots;
    for (unsigned int i = 0; i < size; i++) {
        depots.push_back(out[i]);
    }
    delete[] out;
    return depots;
}

std::string Steam::Apps::getAppInstallPath(std::uint32_t appID) {
    auto apps = Steam::Apps::get();
    if (!apps) {
        return "";
    }
    char out[MAX_API_STRING_LENGTH];
    auto size = Steam::get().call<std::uint32_t>("SteamAPI_ISteamApps_GetAppInstallDir", apps, appID, out, MAX_API_STRING_LENGTH);
    if (!size) {
        return "";
    }
    std::string path{out, *size - 1};
    return path;
}

bool Steam::Apps::isAppInstalled(std::uint32_t appID) {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsAppInstalled", false, Steam::Apps::get(), appID);
}

std::uint64_t Steam::Apps::getAppOwner() {
    return steamFunctionWrapper<std::uint64_t>("SteamAPI_ISteamApps_GetAppOwner", 0, Steam::Apps::get());
}

std::string Steam::Apps::getLaunchParameter(std::string_view key) {
    return steamFunctionStringWrapper("SteamAPI_ISteamApps_GetLaunchQueryParam", "", Steam::Apps::get(), key.data());
}

bool Steam::Apps::getDLCDownloadProgress(std::uint32_t appID, std::uint64_t* bytesDownloaded, std::uint64_t* bytesTotal) {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_GetDlcDownloadProgress", false, Steam::Apps::get(), appID, bytesDownloaded, bytesTotal);
}

int Steam::Apps::getAppBuildID() {
    return steamFunctionWrapper<int>("SteamAPI_ISteamApps_GetAppBuildId", 0, Steam::Apps::get());
}

std::uint64_t Steam::Apps::getFileDetails(std::string_view filename) {
    return steamFunctionWrapper<std::uint64_t>("SteamAPI_ISteamApps_GetFileDetails", 0, Steam::Apps::get(), filename.data());
}

bool Steam::Apps::isSubscribedFromFamilySharing() {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsSubscribedFromFamilySharing", false, Steam::Apps::get());
}

bool Steam::Apps::isTimedTrial(std::uint32_t* secondsAllowed, std::uint32_t* secondsPlayed) {
    return steamFunctionWrapper<bool>("SteamAPI_ISteamApps_BIsTimedTrial", false, Steam::Apps::get(), secondsAllowed, secondsPlayed);
}

// -------------------------------- UGC -------------------------------- //

void* Steam::UGC::get() {
    if (auto ugc = Steam::get().call<void*>("SteamAPI_SteamUGC_v016"))
        return *ugc;
    return nullptr;
}
