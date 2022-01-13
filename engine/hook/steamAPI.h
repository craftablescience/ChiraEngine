#pragma once

#include <string>
#include <vector>
#include <utility/primitiveTypes.h>

namespace libloader {
    class library;
}

namespace steam {
    // Singleton types
    struct ISteamClient    {};
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

    // Structs
    struct CallbackMessage {
        std::int32_t user;
        int callbackType;
        std::uint8_t* param;
        int size;
    };

    struct Callbacks {
    private:
        static constexpr const int callbackBaseIdUser = 100;
        static constexpr const int callbackBaseIdFriends = 300;
        static constexpr const int callbackBaseIdContentServer = 600;
        static constexpr const int callbackBaseIdUtils = 700;
        static constexpr const int callbackBaseIdApps = 1000;
        static constexpr const int callbackBaseIdUserStats = 1100;
        static constexpr const int callbackBaseId2Async = 1900;
        static constexpr const int callbackBaseIdGameStats = 2000;
        static constexpr const int callbackBaseIdStreamLauncher = 2600;
        static constexpr const int callbackBaseIdController = 2800;
        static constexpr const int callbackBaseIdUGC = 3400;
        static constexpr const int callbackBaseIdStreamClient = 3500;
        static constexpr const int callbackBaseIdAppList = 3900;
        static constexpr const int callbackBaseIdGameNotification = 4400;
        static constexpr const int callbackBaseIdParentalSettings = 5000;
        static constexpr const int callbackBaseIdGameSearch = 5200;
        static constexpr const int callbackBaseIdSTAR = 5500;
    public:
        struct Completed {
            static constexpr const int callbackType = Callbacks::callbackBaseIdUtils + 3;
            std::uint64_t asyncCallbackId;
            int callbackId;
            std::uint32_t size;
        };
    };
}

namespace chira {
    struct SteamAPI {
        static const libloader::library& get();

        struct Client {
            static steam::ISteamClient* get();
            static bool initSteam();
            static bool initialized();
            static void runCallbacks();
            static void shutdown();
        private:
            static inline bool isInitialized = false;
        };

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
            static std::string   getPersonaName();
            static std::uint64_t setPersonaName(const std::string& name);
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
    };
}
