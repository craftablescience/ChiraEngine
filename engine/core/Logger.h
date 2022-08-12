#pragma once

#include <string_view>
#include <vector>
#include <functional>
#include <unordered_map>

#include <utility/UUIDGenerator.h>

namespace chira {

enum class LogType {
    LOG_INFO,           // white
    LOG_INFO_IMPORTANT, // green
    LOG_OUTPUT,         // blue
    LOG_WARNING,        // yellow
    LOG_ERROR           // red
};

class Logger {
    friend class LogChannel;
    using loggingCallback = std::function<void(LogType,std::string_view,std::string_view)>;
public:
    Logger() = delete;
    static uuids::uuid addCallback(const loggingCallback& callback);
    static void runLogHooks(LogType type, std::string_view source, std::string_view message);
    static void removeCallback(const uuids::uuid& id);
private:
    static void log(LogType type, std::string_view source, std::string_view message);

public:
    static inline constexpr std::string_view INFO_PREFIX = "[*]";
    static inline constexpr std::string_view INFO_IMPORTANT_PREFIX = "[!]";
    static inline constexpr std::string_view OUTPUT_PREFIX = "[O]";
    static inline constexpr std::string_view WARNING_PREFIX = "[W]";
    static inline constexpr std::string_view ERROR_PREFIX = "[E]";
private:
    static inline std::unordered_map<uuids::uuid, loggingCallback> callbacks;
};

class LogChannel {
    class LogChannelLogger {
    public:
        constexpr LogChannelLogger(LogType type_, std::string_view source_) : type(type_), source(source_) {}
        inline void operator<<(std::string_view message) const {
            Logger::log(this->type, this->source, message);
        }
    private:
        LogType type;
        std::string_view source;
    };
public:
    constexpr explicit LogChannel(std::string_view name_) : name(name_) {
        this->loggers.emplace_back(LogType::LOG_INFO,           this->name);
        this->loggers.emplace_back(LogType::LOG_INFO_IMPORTANT, this->name);
        this->loggers.emplace_back(LogType::LOG_OUTPUT,         this->name);
        this->loggers.emplace_back(LogType::LOG_WARNING,        this->name);
        this->loggers.emplace_back(LogType::LOG_ERROR,          this->name);
    }
    [[nodiscard]] constexpr std::string_view getName() const {
        return this->name;
    }
    [[nodiscard]] inline constexpr const LogChannelLogger& get(LogType type) const {
        return this->loggers.at(static_cast<int>(type));
    }
    [[nodiscard]] inline constexpr const LogChannelLogger& info() const {
        return this->loggers.at(static_cast<int>(LogType::LOG_INFO));
    }
    [[nodiscard]] inline constexpr const LogChannelLogger& infoImportant() const {
        return this->loggers.at(static_cast<int>(LogType::LOG_INFO_IMPORTANT));
    }
    [[nodiscard]] inline constexpr const LogChannelLogger& output() const {
        return this->loggers.at(static_cast<int>(LogType::LOG_OUTPUT));
    }
    [[nodiscard]] inline constexpr const LogChannelLogger& warning() const {
        return this->loggers.at(static_cast<int>(LogType::LOG_WARNING));
    }
    [[nodiscard]] inline constexpr const LogChannelLogger& error() const {
        return this->loggers.at(static_cast<int>(LogType::LOG_ERROR));
    }
    inline void info(std::string_view message) const {
        this->info() << message;
    }
    inline void infoImportant(std::string_view message) const {
        this->infoImportant() << message;
    }
    inline void output(std::string_view message) const {
        this->output() << message;
    }
    inline void warning(std::string_view message) const {
        this->warning() << message;
    }
    inline void error(std::string_view message) const {
        this->error() << message;
    }
private:
    std::string name;
    std::vector<LogChannelLogger> loggers;
};

} // namespace chira

#define CHIRA_CREATE_LOG(name) static LogChannel LOG_##name{#name}
#define CHIRA_CREATE_LOG_LOCAL(name) LogChannel LOG_##name{#name}
