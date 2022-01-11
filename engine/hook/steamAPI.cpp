#include "steamAPI.h"

#include <libloader.hpp>
#include <resource/provider/filesystemResourceProvider.h>
#include <utility/logger.h>
#include <utility/string/stringSplit.h>

using namespace chira;
using namespace steam;
using namespace libloader;

/// Helper function to stop repeating stuff
template<typename T, typename U, typename... Params>
inline T steamFunctionWrapper(const std::string& function, T defaultValue, U steamSingleton, Params... params) {
    if (steamSingleton) {
        T out = defaultValue;
        SteamAPI::get().call<T>(function, out, steamSingleton, params...);
        return out;
    }
    return defaultValue;
}

/// Helper function to stop repeating stuff
template<typename T, typename... Params>
inline std::string steamFunctionStringWrapper(const std::string& function, const std::string& defaultValue, T steamSingleton, Params... params) {
    if (steamSingleton) {
        const char* out = defaultValue.c_str();
        SteamAPI::get().call<const char*>(function, out, steamSingleton, params...);
        return out ? std::string{out} : defaultValue;
    }
    return defaultValue;
}

const library& SteamAPI::get() {
    static library steamBinary{FILESYSTEM_ROOT_FOLDER + "/engine/bin/steam_api"};
    return steamBinary;
}

#if !defined(_WIN32) && (!defined(__clang__) && (defined(__GNUC__) || defined(__GNUG__)))
#define __cdecl __attribute__((__cdecl__))
#endif
extern "C" void __cdecl steamAPILoggingHook(int severity, const char* description) {
    if (severity < 1)
        Logger::log(LogType::INFO, "Steam", std::string{description});
    else
        Logger::log(LogType::WARNING, "Steam", std::string{description});
}

bool SteamAPI::initSteam() {
    if (SteamAPI::isInitialized)
        return true;
    if (auto utils = SteamAPI::Utils::get())
        SteamAPI::get().call("SteamAPI_ISteamUtils_SetWarningMessageHook", utils, &steamAPILoggingHook);
    bool out;
    if (SteamAPI::get().call<bool>("SteamAPI_Init", out)) {
        SteamAPI::isInitialized = out;
        return out;
    }
    return false;
}

bool SteamAPI::initialized() {
    return SteamAPI::isInitialized;
}

void SteamAPI::runCallbacks() {
    SteamAPI::get().callVoid("SteamAPI_RunCallbacks");
}

void SteamAPI::shutdown() {
    SteamAPI::get().callVoid("SteamAPI_Shutdown");
}

// -------------------------------- USER -------------------------------- //

ISteamUser* SteamAPI::User::get() {
    ISteamUser* user = nullptr;
    SteamAPI::get().call<ISteamUser*>("SteamAPI_SteamUser_v021", user);
    return user;
}

bool SteamAPI::User::isLoggedOn() {
    return steamFunctionWrapper("SteamAPI_ISteamUser_BLoggedOn", false, SteamAPI::User::get());
}

std::uint64_t SteamAPI::User::getSteamID() {
    return steamFunctionWrapper<std::uint64_t>("SteamAPI_ISteamUser_GetSteamID", 0, SteamAPI::User::get());
}

bool SteamAPI::User::isBehindNAT() {
    return steamFunctionWrapper("SteamAPI_ISteamUser_BIsBehindNAT", false, SteamAPI::User::get());
}

int SteamAPI::User::getGameBadgeLevel(bool foil, int series) {
    return steamFunctionWrapper<int>("SteamAPI_ISteamUser_GetGameBadgeLevel", 0, SteamAPI::User::get(), series, foil);
}

int SteamAPI::User::getPlayerSteamLevel() {
    return steamFunctionWrapper<int>("SteamAPI_ISteamUser_GetPlayerSteamLevel", -1, SteamAPI::User::get());
}

bool SteamAPI::User::isPhoneVerified() {
    return steamFunctionWrapper("SteamAPI_ISteamUser_BIsPhoneVerified", false, SteamAPI::User::get());
}

bool SteamAPI::User::isTwoFactorAuthenticationEnabled() {
    return steamFunctionWrapper("SteamAPI_ISteamUser_BIsTwoFactorEnabled", false, SteamAPI::User::get());
}

bool SteamAPI::User::isPhoneIdentifying() {
    return steamFunctionWrapper("SteamAPI_ISteamUser_BIsPhoneIdentifying", false, SteamAPI::User::get());
}

bool SteamAPI::User::isPhoneRequiringVerification() {
    return steamFunctionWrapper("SteamAPI_ISteamUser_BIsPhoneRequiringVerification", false, SteamAPI::User::get());
}

std::uint64_t SteamAPI::User::getMarketEligibility() {
    return steamFunctionWrapper<std::uint64_t>("SteamAPI_ISteamUser_GetMarketEligibility", 0, SteamAPI::User::get());
}

// -------------------------------- FRIENDS -------------------------------- //

ISteamFriends* SteamAPI::Friends::get() {
    ISteamFriends* friends = nullptr;
    SteamAPI::get().call<ISteamFriends*>("SteamAPI_SteamFriends_v017", friends);
    return friends;
}

std::string SteamAPI::Friends::getPersonaName() {
    return steamFunctionStringWrapper("SteamAPI_ISteamFriends_GetPersonaName", "", SteamAPI::Friends::get());
}

// -------------------------------- UTILS -------------------------------- //

ISteamUtils* SteamAPI::Utils::get() {
    ISteamUtils* utils = nullptr;
    SteamAPI::get().call<ISteamUtils*>("SteamAPI_SteamUtils_v010", utils);
    return utils;
}

std::uint32_t SteamAPI::Utils::getSecondsSinceAppActive() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetSecondsSinceAppActive", 0, SteamAPI::Utils::get());
}

std::uint32_t SteamAPI::Utils::getSecondsSinceComputerActive() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetSecondsSinceComputerActive", 0, SteamAPI::Utils::get());
}

std::uint32_t SteamAPI::Utils::getServerRealTime() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetServerRealTime", 0, SteamAPI::Utils::get());
}

std::string SteamAPI::Utils::getIPCountry() {
    return steamFunctionStringWrapper("SteamAPI_ISteamUtils_GetIPCountry", "", SteamAPI::Utils::get());
}

bool SteamAPI::Utils::getImageSize(int imageID, std::uint32_t* width, std::uint32_t* height) {
    return steamFunctionWrapper("SteamAPI_ISteamUtils_GetImageSize", false, SteamAPI::Utils::get(), imageID, width, height);
}

bool SteamAPI::Utils::getImageRGBA(int imageID, std::uint8_t* imageBuffer, int imageBufferSize) {
    return steamFunctionWrapper("SteamAPI_ISteamUtils_GetImageRGBA", false, SteamAPI::Utils::get(), imageID, imageBuffer, imageBufferSize);
}

std::uint8_t SteamAPI::Utils::getCurrentBatteryPower() {
    return steamFunctionWrapper<std::uint8_t>("SteamAPI_ISteamUtils_GetCurrentBatteryPower", 0, SteamAPI::Utils::get());
}

std::uint32_t SteamAPI::Utils::getAppID() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetAppID", 0, SteamAPI::Utils::get());
}

void SteamAPI::Utils::setOverlayNotificationPosition(NotificationPosition position) {
    if (auto utils = SteamAPI::Utils::get())
        SteamAPI::get().callVoid("SteamAPI_ISteamUtils_SetOverlayNotificationPosition", utils, position);
}

std::uint32_t SteamAPI::Utils::getIPCCallCount() {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamUtils_GetIPCCallCount", 0, SteamAPI::Utils::get());
}

bool SteamAPI::Utils::isOverlayEnabled() {
    return steamFunctionWrapper("SteamAPI_ISteamUtils_IsOverlayEnabled", false, SteamAPI::Utils::get());
}

bool SteamAPI::Utils::isRunningInVR() {
    return steamFunctionWrapper("SteamAPI_ISteamUtils_IsSteamRunningInVR", false, SteamAPI::Utils::get());
}

void SteamAPI::Utils::setOverlayNotificationInset(int horizontalInset, int verticalInset) {
    if (auto utils = SteamAPI::Utils::get())
        SteamAPI::get().callVoid("SteamAPI_ISteamUtils_SetOverlayNotificationInset", utils, horizontalInset, verticalInset);
}

bool SteamAPI::Utils::isBigPictureModeOn() {
    return steamFunctionWrapper("SteamAPI_ISteamUtils_IsSteamInBigPictureMode", false, SteamAPI::Utils::get());
}

void SteamAPI::Utils::startVRDashboard() {
    if (auto utils = SteamAPI::Utils::get())
        SteamAPI::get().callVoid("SteamAPI_ISteamUtils_StartVRDashboard", utils);
}

bool SteamAPI::Utils::isVRHeadsetStreamingEnabled() {
    return steamFunctionWrapper("SteamAPI_ISteamUtils_IsVRHeadsetStreamingEnabled", false, SteamAPI::Utils::get());
}

void SteamAPI::Utils::setVRHeadsetStreamingEnabled(bool enabled) {
    if (auto utils = SteamAPI::Utils::get())
        SteamAPI::get().callVoid("SteamAPI_ISteamUtils_SetVRHeadsetStreamingEnabled", utils, enabled);
}

bool SteamAPI::Utils::isRunningOnSteamDeck() {
    return steamFunctionWrapper("SteamAPI_ISteamUtils_IsSteamRunningOnSteamDeck", false, SteamAPI::Utils::get());
}

void SteamAPI::Utils::setGameLauncherMode(bool launcherMode) {
    if (auto utils = SteamAPI::Utils::get())
        SteamAPI::get().callVoid("SteamAPI_ISteamUtils_SetGameLauncherMode", utils, launcherMode);
}

// -------------------------------- USER STATS -------------------------------- //

ISteamUserStats* SteamAPI::UserStats::get() {
    ISteamUserStats* userStats = nullptr;
    SteamAPI::get().call<ISteamUserStats*>("SteamAPI_SteamUserStats_v012", userStats);
    return userStats;
}

// -------------------------------- APPS -------------------------------- //

ISteamApps* SteamAPI::Apps::get() {
    ISteamApps* apps = nullptr;
    SteamAPI::get().call<ISteamApps*>("SteamAPI_SteamApps_v008", apps);
    return apps;
}

bool SteamAPI::Apps::userOwnsThisAppID() {
    return steamFunctionWrapper("SteamAPI_ISteamApps_BIsSubscribed", false, SteamAPI::Apps::get());
}

bool SteamAPI::Apps::isLowViolence() {
    return steamFunctionWrapper("SteamAPI_ISteamApps_BIsLowViolence", false, SteamAPI::Apps::get());
}

bool SteamAPI::Apps::isCybercafe() {
    return steamFunctionWrapper("SteamAPI_ISteamApps_BIsCybercafe", false, SteamAPI::Apps::get());
}

bool SteamAPI::Apps::isVACBanned() {
    return steamFunctionWrapper("SteamAPI_ISteamApps_BIsVACBanned", false, SteamAPI::Apps::get());
}

std::string SteamAPI::Apps::getCurrentGameLanguage() {
    return steamFunctionStringWrapper("SteamAPI_ISteamApps_GetCurrentGameLanguage", "", SteamAPI::Apps::get());
}

std::vector<std::string> SteamAPI::Apps::getAvailableGameLanguages() {
    return split(steamFunctionStringWrapper("SteamAPI_ISteamApps_GetAvailableGameLanguages", "", SteamAPI::Apps::get()), ',');
}

bool SteamAPI::Apps::isSubscribedApp(std::uint32_t appID) {
    return steamFunctionWrapper("SteamAPI_ISteamApps_BIsSubscribedApp", false, SteamAPI::Apps::get(), appID);
}

bool SteamAPI::Apps::isDLCInstalled(std::uint32_t appID) {
    return steamFunctionWrapper("SteamAPI_ISteamApps_BIsDlcInstalled", false, SteamAPI::Apps::get(), appID);
}

std::uint32_t SteamAPI::Apps::getEarliestPurchaseUnixTime(std::uint32_t appID) {
    return steamFunctionWrapper<std::uint32_t>("SteamAPI_ISteamApps_GetEarliestPurchaseUnixTime", 0, SteamAPI::Apps::get(), appID);
}

bool SteamAPI::Apps::isSubscribedFromFreeWeekend() {
    return steamFunctionWrapper("SteamAPI_ISteamApps_BIsSubscribedFromFreeWeekend", false, SteamAPI::Apps::get());
}

int SteamAPI::Apps::getDLCCount() {
    return steamFunctionWrapper<int>("SteamAPI_ISteamApps_GetDLCCount", 0, SteamAPI::Apps::get());
}

// -------------------------------- UGC -------------------------------- //

ISteamUGC* SteamAPI::UGC::get() {
    ISteamUGC* ugc = nullptr;
    SteamAPI::get().call<ISteamUGC*>("SteamAPI_SteamUGC_v016", ugc);
    return ugc;
}
