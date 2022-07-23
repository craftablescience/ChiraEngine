#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <type_traits>
#include <vector>

#include <loader/settings/JSONSettingsLoader.h>
#include "Assertions.h"
#include "Logger.h"

namespace chira {

enum ConFlags {
    CON_FLAG_NONE   = 1 << 0, // None
    CON_FLAG_CHEAT  = 1 << 1, // Cheat-protected
    CON_FLAG_HIDDEN = 1 << 2, // Doesn't show up in search
    CON_FLAG_CACHE  = 1 << 3, // Value is saved at exit and loaded at start (useless for commands)
};

class ConCommand {
public:
    using CallbackArgs = const std::vector<std::string>&;

    ConCommand(std::string name_, const std::function<void()>& callback_, int flags_ = CON_FLAG_NONE);
    ConCommand(std::string name_, std::function<void(CallbackArgs)> callback_, int flags_ = CON_FLAG_NONE);
    ConCommand(std::string name_, std::string description_, const std::function<void()>& callback_, int flags_ = CON_FLAG_NONE);
    ConCommand(std::string name_, std::string description_, std::function<void(CallbackArgs)> callback_, int flags_ = CON_FLAG_NONE);
    ~ConCommand();
    [[nodiscard]] std::string_view getName() const;
    [[nodiscard]] std::string_view getDescription() const;
    [[nodiscard]] bool hasFlag(ConFlags flag) const;
    void fire(CallbackArgs args);
    [[nodiscard]] virtual explicit operator std::string() const {
        return std::string{this->getName()} + ": function - " + this->getDescription().data();
    }
private:
    std::string name;
    std::string description;
    int flags;
    std::function<void(CallbackArgs)> callback;
};

class ConCommandRegistry {
    friend ConCommand;
    friend class ConVar;
public:
    ConCommandRegistry() = delete;
    [[nodiscard]] static bool hasConCommand(std::string_view name);
    [[nodiscard]] static ConCommand* getConCommand(std::string_view name);
    [[nodiscard]] static std::vector<std::string> getConCommandList();
private:
    static std::vector<ConCommand*>& getConCommands();
    static bool registerConCommand(ConCommand* concommand);
    static void deregisterConCommand(ConCommand* concommand);
};

// Must be delared before ConVar
class ConVarRegistry {
    friend class ConVar;
public:
    ConVarRegistry() = delete;
    [[nodiscard]] static bool hasConVar(std::string_view name);
    [[nodiscard]] static ConVar* getConVar(std::string_view name);
    [[nodiscard]] static std::vector<std::string> getConVarList();
private:
    static std::vector<ConVar*>& getConVars();
    static JSONSettingsLoader& getConVarCache();
    static bool registerConVar(ConVar* convar);
    static void deregisterConVar(ConVar* convar);
};

// These are all the types that can currently be serialized into JSON
template<typename T>
concept ConVarValidType = std::same_as<bool, T> ||
                          std::same_as<int, T> ||
                          std::same_as<double, T> ||
                          std::same_as<std::string, T>;

// Don't make the ConVar class a template :)
enum class ConVarType {
    BOOLEAN,
    INTEGER,
    DOUBLE,
    STRING,
};

class ConVar : public ConCommand {
public:
    using CallbackArg = std::string_view;

    ConVar(std::string name_, ConVarValidType auto defaultValue, int flags_ = CON_FLAG_NONE, std::function<void(CallbackArg)> onChanged = [](CallbackArg) {})
        : ConCommand(std::move(name_), [](ConCommand::CallbackArgs) {}, flags_)
        , changedCallback(std::move(onChanged)) {
        if constexpr (std::is_same_v<decltype(defaultValue), std::string>) {
            this->value = std::move(defaultValue);
            this->type = ConVarType::STRING;
        } else {
            this->value = std::to_string(defaultValue);
            if constexpr (std::is_same_v<decltype(defaultValue), bool>) {
                this->type = ConVarType::BOOLEAN;
            }
            else if constexpr (std::is_same_v<decltype(defaultValue), int>) {
                this->type = ConVarType::INTEGER;
            }
            else /* if constexpr (std::is_same_v<decltype(defaultValue), double>) */ {
                this->type = ConVarType::DOUBLE;
            }
        }
        // undo parent class ctor
        ConCommandRegistry::deregisterConCommand(this);
        runtime_assert(ConVarRegistry::registerConVar(this), "This convar already exists! This will cause problems...");
    }

    ConVar(std::string name_, ConVarValidType auto defaultValue, std::string description_, int flags_ = CON_FLAG_NONE, std::function<void(CallbackArg)> onChanged = [](CallbackArg) {})
        : ConCommand(std::move(name_), std::move(description_), [](ConCommand::CallbackArgs) {}, flags_)
        , changedCallback(std::move(onChanged)) {
        if constexpr (std::is_same_v<decltype(defaultValue), std::string>) {
            this->value = std::move(defaultValue);
            this->type = ConVarType::STRING;
        } else {
            this->value = std::to_string(defaultValue);
            if constexpr (std::is_same_v<decltype(defaultValue), bool>) {
                this->type = ConVarType::BOOLEAN;
            } else if constexpr (std::is_same_v<decltype(defaultValue), int>) {
                this->type = ConVarType::INTEGER;
            } else /* if constexpr (std::is_same_v<decltype(defaultValue), double>) */ {
                this->type = ConVarType::DOUBLE;
            }
        }
        // undo parent class ctor
        ConCommandRegistry::deregisterConCommand(this);
        runtime_assert(ConVarRegistry::registerConVar(this), "This convar already exists! This will cause problems...");
    }

    ~ConVar() {
        ConVarRegistry::deregisterConVar(this);
    }

    [[nodiscard]] ConVarType getType() const {
        return this->type;
    }

    [[nodiscard]] std::string_view getTypeAsString() const {
        switch (this->type) {
            case ConVarType::BOOLEAN:
                return "boolean";
            case ConVarType::INTEGER:
                return "integer";
            case ConVarType::DOUBLE:
                return "double";
            case ConVarType::STRING:
                return "string";
        }
        return "";
    }

    template<typename T>
    requires ConVarValidType<T>
    inline T getValue() const {
        if constexpr (std::is_same_v<T, std::string>) {
            return this->value;
        } else {
            if (this->type == ConVarType::STRING) {
                return static_cast<T>(this->value.size());
            } else if (this->type == ConVarType::DOUBLE) {
                return static_cast<T>(std::stod(this->value));
            } else {
                return static_cast<T>(std::stoi(this->value));
            }
        }
    }

    void setValue(ConVarValidType auto newValue) {
        if (this->hasFlag(CON_FLAG_CHEAT) && !ConVar::areCheatsEnabled()) {
            Logger::log(LogType::LOG_ERROR, "ConVar", "Cannot set value of cheat-protected convar with cheats disabled.");
            return;
        }
        if constexpr (std::is_same_v<decltype(newValue), std::string>) {
            switch (this->type) {
                case ConVarType::BOOLEAN:
                case ConVarType::INTEGER:
                    try {
                        this->value = std::to_string(std::stoi(newValue));
                    } catch (const std::invalid_argument&) {
                        this->value = std::to_string(newValue.size());
                    }
                    break;
                case ConVarType::DOUBLE:
                    try {
                        this->value = std::to_string(std::stod(newValue));
                    } catch (const std::invalid_argument&) {
                        this->value = std::to_string(static_cast<double>(newValue.size()));
                    }
                    break;
                default:
                case ConVarType::STRING:
                    this->value = newValue;
                    break;
            }
        } else {
            switch (this->type) {
                case ConVarType::BOOLEAN:
                    this->value = std::to_string(static_cast<bool>(newValue));
                    break;
                case ConVarType::INTEGER:
                    this->value = std::to_string(static_cast<int>(newValue));
                    break;
                case ConVarType::DOUBLE:
                    this->value = std::to_string(static_cast<double>(newValue));
                    break;
                default:
                case ConVarType::STRING:
                    this->value = std::to_string(newValue);
                    break;
            }
        }
        this->changedCallback(this->value);
    }

    [[nodiscard]] explicit operator std::string() const override {
        return std::string{this->getName()} + ": " + this->getTypeAsString().data() + " - " + this->getDescription().data();
    }

    [[nodiscard]] static bool areCheatsEnabled();
private:
    std::function<void(CallbackArg)> changedCallback;
    std::string value;
    ConVarType type;

    using ConCommand::fire;
};

} // namespace chira
