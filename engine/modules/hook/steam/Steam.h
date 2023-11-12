#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <core/Platform.h>

namespace libloader {

class library;

} // namespace libloader

namespace chira::Steam {

constexpr std::int32_t MAX_API_STRING_LENGTH = 1024;

// -- Enums --
enum class NotificationPosition : int {
    TOP_LEFT     = 0,
    TOP_RIGHT    = 1,
    BOTTOM_LEFT  = 2,
    BOTTOM_RIGHT = 3,
};

// Thank you GodotSteam devs for the very nice enum (under MIT no less)
// Made some changes to it but that's where this comes from
enum class Result : int {
    OK                                            =   1,
    FAIL                                          =   2,
    NO_CONNECTION                                 =   3,
    INVALID_PASSWORD                              =   5,
    LOGGED_IN_ELSEWHERE                           =   6,
    INVALID_PROTOCOL_VER                          =   7,
    INVALID_PARAM                                 =   8,
    FILE_NOT_FOUND                                =   9,
    BUSY                                          =  10,
    INVALID_STATE                                 =  11,
    INVALID_NAME                                  =  12,
    INVALID_EMAIL                                 =  13,
    DUPLICATE_NAME                                =  14,
    ACCESS_DENIED                                 =  15,
    TIMEOUT                                       =  16,
    BANNED                                        =  17,
    ACCOUNT_NOT_FOUND                             =  18,
    INVALID_STEAM_ID                              =  19,
    SERVICE_UNAVAILABLE                           =  20,
    NOT_LOGGED_ON                                 =  21,
    PENDING                                       =  22,
    ENCRYPTION_FAILURE                            =  23,
    INSUFFICIENT_PRIVILEGE                        =  24,
    LIMIT_EXCEEDED                                =  25,
    REVOKED                                       =  26,
    EXPIRED                                       =  27,
    ALREADY_REDEEMED                              =  28,
    DUPLICATE_REQUEST                             =  29,
    ALREADY_OWNED                                 =  30,
    IP_NOT_FOUND                                  =  31,
    PERSIST_FAILED                                =  32,
    LOCKING_FAILED                                =  33,
    LOG_ON_SESSION_REPLACED                       =  34,
    CONNECT_FAILED                                =  35,
    HANDSHAKE_FAILED                              =  36,
    IO_FAILURE                                    =  37,
    REMOTE_DISCONNECT                             =  38,
    SHOPPING_CART_NOT_FOUND                       =  39,
    BLOCKED                                       =  40,
    IGNORED                                       =  41,
    NO_MATCH                                      =  42,
    ACCOUNT_DISABLED                              =  43,
    SERVICE_READ_ONLY                             =  44,
    ACCOUNT_NOT_FEATURED                          =  45,
    ADMINISTRATOR_OK                              =  46,
    CONTENT_VERSION                               =  47,
    TRY_ANOTHER_CM                                =  48,
    PASSWORD_REQUIRED_TO_KICK_SESSION             =  49,
    ALREADY_LOGGED_IN_ELSEWHERE                   =  50,
    SUSPENDED                                     =  51,
    CANCELLED                                     =  52,
    DATA_CORRUPTION                               =  53,
    DISK_FULL                                     =  54,
    REMOTE_CALL_FAILED                            =  55,
    PASSWORD_UNSET                                =  56,
    EXTERNAL_ACCOUNT_UNLINKED                     =  57,
    PSN_TICKET_INVALID                            =  58,
    EXTERNAL_ACCOUNT_ALREADY_LINKED               =  59,
    REMOTE_FILE_CONFLICT                          =  60,
    ILLEGAL_PASSWORD                              =  61,
    SAME_AS_PREVIOUS_VALUE                        =  62,
    ACCOUNT_LOG_ON_DENIED                         =  63,
    CANNOT_USE_OLD_PASSWORD                       =  64,
    INVALID_LOGIN_AUTH_CODE                       =  65,
    ACCOUNT_LOG_ON_DENIED_NO_MAIL                 =  66,
    HARDWARE_NOT_CAPABLE_OF_IPT                   =  67,
    IPT_INIT_ERROR                                =  68,
    PARENTAL_CONTROL_RESTRICTED                   =  69,
    FACEBOOK_QUERY_ERROR                          =  70,
    EXPIRED_LOGIN_AUTH_CODE                       =  71,
    IP_LOGIN_RESTRICTION_FAILED                   =  72,
    ACCOUNT_LOCKED_DOWN                           =  73,
    ACCOUNT_LOG_ON_DENIED_VERIFIED_EMAIL_REQUIRED =  74,
    NO_MATCHING_URL                               =  75,
    BAD_RESPONSE                                  =  76,
    REQUIRE_PASSWORD_REENTRY                      =  77,
    VALUE_OUT_OF_RANGE                            =  78,
    UNEXPECTED_ERROR                              =  79,
    DISABLED                                      =  80,
    INVALID_CEG_SUBMISSION                        =  81,
    RESTRICTED_DEVICE                             =  82,
    REGION_LOCKED                                 =  83,
    RATE_LIMIT_EXCEEDED                           =  84,
    ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR          =  85,
    ITEM_DELETED                                  =  86,
    ACCOUNT_LOGIN_DENIED_THROTTLE                 =  87,
    TWO_FACTOR_CODE_MISMATCH                      =  88,
    TWO_FACTOR_ACTIVATION_CODE_MISMATCH           =  89,
    ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS       =  90,
    NOT_MODIFIED                                  =  91,
    NO_MOBILE_DEVICE                              =  92,
    TIME_NOT_SYNCED                               =  93,
    SMS_CODE_FAILED                               =  94,
    ACCOUNT_LIMIT_EXCEEDED                        =  95,
    ACCOUNT_ACTIVITY_LIMIT_EXCEEDED               =  96,
    PHONE_ACTIVITY_LIMIT_EXCEEDED                 =  97,
    REFUND_TO_WALLET                              =  98,
    EMAIL_SEND_FAILURE                            =  99,
    NOT_SETTLED                                   = 100,
    NEEDS_CAPTCHA                                 = 101,
    GSLT_DENIED                                   = 102,
    GS_OWNER_DENIED                               = 103,
    INVALID_ITEM_TYPE                             = 104,
    IP_BANNED                                     = 105,
    GSLT_EXPIRED                                  = 106,
    INSUFFICIENT_FUNDS                            = 107,
    TOO_MANY_PENDING                              = 108,
};

enum class CallbackMessageType : int {
    GAME_OVERLAY_ACTIVATED =  331,
    COMPLETED              =  703,
    DLC_INSTALLED          = 1005,
    FILE_DETAILS_RESULT    = 1023,
};

// -- Structs --
struct CallbackMessage {
    std::int32_t user;
    int callbackType;
    std::uint8_t* callback;
    int callbackSize;
};

struct Callbacks {
    // For simple structs with one member variable, the variable is passed to the event
    // Otherwise, the struct itself is passed
#pragma pack(push, 8)
    // Friends
    struct GameOverlayActivated {
        std::uint8_t active;
    };

    // Utils
    struct Completed {
        std::uint64_t asyncCallbackId;
        int           callbackId;
        std::uint32_t size;
    };

    // Apps
    struct DLCInstalled {
        std::uint32_t appID;
    };
    struct FileDetailsResult {
        Result        result;
        std::uint64_t fileSize;
        std::uint8_t  fileSHAHash[20];
        std::uint32_t fileFlags;
    };
#pragma pack(pop)
};

const libloader::library& get();

/// For debug use only!
void generateAppIDFile(unsigned int appID);

namespace Client {

void* get();
bool  initSteam();
void  runCallbacks();
void  shutdown();

} // namespace Client

namespace User {

void*         get();
bool          isLoggedOn();
std::uint64_t getSteamID();
bool          isBehindNAT();
int           getGameBadgeLevel(bool foil = false, int series = 1);
int           getPlayerSteamLevel();
bool          isPhoneVerified();
bool          isTwoFactorAuthenticationEnabled();
bool          isPhoneIdentifying();
bool          isPhoneRequiringVerification();
std::uint64_t getMarketEligibility();

} // namespace User

namespace Friends {

void*         get();
std::string   getPersonaName();
std::uint64_t setPersonaName(std::string_view name);

} // namespace Friends

namespace Utils {

void*         get();
std::uint32_t getSecondsSinceAppActive();
std::uint32_t getSecondsSinceComputerActive();
std::uint32_t getServerRealTime();
std::string   getIPCountry();
bool          getImageSize(int imageID, std::uint32_t* width, std::uint32_t* height);
bool          getImageRGBA(int imageID, std::uint8_t* imageBuffer, int imageBufferSize);
std::uint8_t  getCurrentBatteryPower();
std::uint32_t getAppID();
void          setOverlayNotificationPosition(NotificationPosition position);
std::uint32_t getIPCCallCount();
bool          isOverlayEnabled();
bool          isRunningInVR();
void          setOverlayNotificationInset(int horizontalInset, int verticalInset);
bool          isBigPictureModeOn();
void          startVRDashboard();
bool          isVRHeadsetStreamingEnabled();
void          setVRHeadsetStreamingEnabled(bool enabled);
bool          isRunningOnSteamDeck();
void          setGameLauncherMode(bool launcherMode);

} // namespace Utils

namespace UserStats {

void* get();
// todo: implement wrappers + callbacks

} // namespace UserStats

namespace Apps {

void*                      get();
bool                       userOwnsThisAppID();
bool                       isLowViolence();
bool                       isCybercafe();
bool                       isVACBanned();
std::string                getCurrentGameLanguage();
std::vector<std::string>   getAvailableGameLanguages();
bool                       isSubscribedApp(std::uint32_t appID);
bool                       isDLCInstalled(std::uint32_t appID);
std::uint32_t              getEarliestPurchaseUnixTime(std::uint32_t appID);
bool                       isSubscribedFromFreeWeekend();
int                        getDLCCount();
bool                       getDLCData(int dlc, std::uint32_t* appID, bool* available, std::string& name);
void                       installDLC(std::uint32_t appID);
void                       uninstallDLC(std::uint32_t appID);
std::string                getCurrentBranch();
bool                       markContentCorrupt(bool missingFilesOnly);
std::vector<std::uint32_t> getInstalledDepots(std::uint32_t appID);
std::string                getAppInstallPath(std::uint32_t appID);
bool                       isAppInstalled(std::uint32_t appID);
std::uint64_t              getAppOwner();
std::string                getLaunchParameter(std::string_view key);
bool                       getDLCDownloadProgress(std::uint32_t appID, std::uint64_t* bytesDownloaded, std::uint64_t* bytesTotal);
int                        getAppBuildID();
std::uint64_t              getFileDetails(std::string_view filename);
bool                       isSubscribedFromFamilySharing();
bool                       isTimedTrial(std::uint32_t* secondsAllowed, std::uint32_t* secondsPlayed);

} // namespace Apps

namespace UGC {

void* get();
// todo: implement wrappers + callbacks

} // namespace UGC

} // namespace chira::Steam
