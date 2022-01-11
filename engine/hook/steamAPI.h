#pragma once

#include <string>
#include <vector>
#include <utility/primitiveTypes.h>

namespace libloader {
    class library;
}

namespace steam {
    // Singleton types
    struct ISteamUser      {};
    struct ISteamFriends   {};
    struct ISteamUtils     {};
    struct ISteamUserStats {};
    struct ISteamApps      {};
    struct ISteamUGC       {};

    // Enums
    enum class NotificationPosition : int {
        TOP_LEFT     = 0,
        TOP_RIGHT    = 1,
        BOTTOM_LEFT  = 2,
        BOTTOM_RIGHT = 3,
    };
}

namespace chira {
    struct SteamAPI {
        static const libloader::library& get();

        // Startup & shutdown
        static bool initSteam();
        static bool initialized();
        static void runCallbacks();
        static void shutdown();

        struct User {
            static steam::ISteamUser* get();
            static bool          isLoggedOn();
            static std::uint64_t getSteamID();
            static bool          isBehindNAT();
            static int           getGameBadgeLevel(bool foil = false, int series = 1);
            static int           getPlayerSteamLevel();
            static bool          isPhoneVerified();
            static bool          isTwoFactorAuthenticationEnabled();
            static bool          isPhoneIdentifying();
            static bool          isPhoneRequiringVerification();
            static std::uint64_t getMarketEligibility();
        };

        struct Friends {
            static steam::ISteamFriends* get();
            static std::string getPersonaName();
            // todo: wrap around the rest
        };

        struct Utils {
            static steam::ISteamUtils* get();
            static std::uint32_t getSecondsSinceAppActive();
            static std::uint32_t getSecondsSinceComputerActive();
            static std::uint32_t getServerRealTime();
            static std::string   getIPCountry();
            static bool          getImageSize(int imageID, std::uint32_t* width, std::uint32_t* height);
            static bool          getImageRGBA(int imageID, std::uint8_t* imageBuffer, int imageBufferSize);
            static std::uint8_t  getCurrentBatteryPower();
            static std::uint32_t getAppID();
            static void          setOverlayNotificationPosition(steam::NotificationPosition position);
            static std::uint32_t getIPCCallCount();
            static bool          isOverlayEnabled();
            static bool          isRunningInVR();
            static void          setOverlayNotificationInset(int horizontalInset, int verticalInset);
            static bool          isBigPictureModeOn();
            static void          startVRDashboard();
            static bool          isVRHeadsetStreamingEnabled();
            static void          setVRHeadsetStreamingEnabled(bool enabled);
            static bool          isRunningOnSteamDeck();
            static void          setGameLauncherMode(bool launcherMode);
        };

        struct UserStats {
            static steam::ISteamUserStats* get();
            // todo: implement wrappers
        };

        struct Apps {
            static steam::ISteamApps*         get();
            static bool                       userOwnsThisAppID();
            static bool                       isLowViolence();
            static bool                       isCybercafe();
            static bool                       isVACBanned();
            static std::string                getCurrentGameLanguage();
            static std::vector<std::string>   getAvailableGameLanguages();
            static bool                       isSubscribedApp(std::uint32_t appID);
            static bool                       isDLCInstalled(std::uint32_t appID);
            static std::uint32_t              getEarliestPurchaseUnixTime(std::uint32_t appID);
            static bool                       isSubscribedFromFreeWeekend();
            static int                        getDLCCount();
            static bool                       getDLCData(int dlc, std::uint32_t* appID, bool* available, std::string& name);
            static void                       installDLC(std::uint32_t appID);
            static void                       uninstallDLC(std::uint32_t appID);
            static std::string                getCurrentBranch();
            static bool                       markContentCorrupt(bool missingFilesOnly);
            static std::vector<std::uint32_t> getInstalledDepots(std::uint32_t appID);
            static std::string                getAppInstallPath(std::uint32_t appID);
            static bool                       isAppInstalled(std::uint32_t appID);
            static std::uint64_t              getAppOwner();
            static std::string                getLaunchParameter(const std::string& key);
            static bool                       getDLCDownloadProgress(std::uint32_t appID, std::uint64_t* bytesDownloaded, std::uint64_t* bytesTotal);
            static int                        getAppBuildID();
            static std::uint64_t              getFileDetails(const std::string& filename);
            static bool                       isSubscribedFromFamilySharing();
            static bool                       isTimedTrial(std::uint32_t* secondsAllowed, std::uint32_t* secondsPlayed);
        };

        struct UGC {
            static steam::ISteamUGC* get();
            // todo: implement wrappers
        };
    private:
        static inline bool isInitialized = false;
    };
}
