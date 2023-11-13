#pragma once

#include <exception>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "file/JSONConfigFile.h"
#include "../debug/Assertions.h"
#include "../debug/Logger.h"
#include "../utility/NoCopyOrMove.h"

CHIRA_GET_LOG(CONENTRY);

namespace chira {

enum ConFlags {
    CON_FLAG_NONE     =      0, // None
    CON_FLAG_CHEAT    = 1 << 0, // Cheat-protected
    CON_FLAG_HIDDEN   = 1 << 1, // Doesn't show up in search
    CON_FLAG_CACHE    = 1 << 2, // Value is saved at exit and loaded at start (useless for concommands)
    CON_FLAG_READONLY = 1 << 3, // ConVar cannot be changed in the CONSOLE. Still modifiable in code (useless for concommands)
    CON_FLAG_DEVONLY  = 1 << 4, // Reserved for possible future use
};

class ConEntry : public NoCopyOrMove {
public:
    ConEntry(std::string name_, std::string description_, int flags_ = CON_FLAG_NONE);

    virtual ~ConEntry() = default;

    [[nodiscard]] std::string_view getName() const;

    [[nodiscard]] std::string_view getDescription() const;

    [[nodiscard]] bool hasFlag(ConFlags flag) const;

protected:
    std::string name;
    std::string description;
    int flags;
};

class ConCommand : public ConEntry {
public:
    using CallbackArgs = const std::vector<std::string>&;

    ConCommand(std::string name_, const std::function<void()>& callback_, int flags_ = CON_FLAG_NONE);
    ConCommand(std::string name_, std::function<void(CallbackArgs)> callback_, int flags_ = CON_FLAG_NONE);
    ConCommand(std::string name_, std::string description_, const std::function<void()>& callback_, int flags_ = CON_FLAG_NONE);
    ConCommand(std::string name_, std::string description_, std::function<void(CallbackArgs)> callback_, int flags_ = CON_FLAG_NONE);

    ~ConCommand() override;

    void fire(CallbackArgs args);

    [[nodiscard]] explicit inline operator std::string() const {
        return std::string{this->getName()} + " - " + this->getDescription().data();
    }

private:
    std::function<void(CallbackArgs)> callback;
};

// Registry be declared before ConVar because of the magic of templates
class ConVar;

class ConEntryRegistry : public NoCopyOrMove {
    friend ConCommand;
    friend ConVar;

public:
    ConEntryRegistry() = delete;

    [[nodiscard]] static bool hasConCommand(std::string_view name);

    [[nodiscard]] static ConCommand* getConCommand(std::string_view name);

    [[nodiscard]] static std::vector<std::string> getConCommandList();

    [[nodiscard]] static bool hasConVar(std::string_view name);

    [[nodiscard]] static ConVar* getConVar(std::string_view name);

    [[nodiscard]] static std::vector<std::string> getConVarList();

	static void initializeConVarCache(ConVar* initializeOnlyMe = nullptr);

private:
    static std::vector<ConCommand*>& getConCommands();

    static bool registerConCommand(ConCommand* concommand);

    static void deregisterConCommand(ConCommand* concommand);

    static std::vector<ConVar*>& getConVars();

    static IConfigFile& getConVarCache();

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

class ConVar : public ConEntry {
public:
    using CallbackArg = std::string_view;

    ConVar(std::string name_, ConVarValidType auto defaultValue, int flags_ = CON_FLAG_NONE, std::function<void(CallbackArg)> onChanged = [](CallbackArg) {})
            : ConEntry(std::move(name_), "No description provided.", flags_)
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
        chiraAssert(ConEntryRegistry::registerConVar(this), "This ConVar already exists!");
    }

    ConVar(std::string name_, ConVarValidType auto defaultValue, std::string description_, int flags_ = CON_FLAG_NONE, std::function<void(CallbackArg)> onChanged = [](CallbackArg) {})
            : ConEntry(std::move(name_), std::move(description_), flags_)
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
        chiraAssert(ConEntryRegistry::registerConVar(this), "This ConVar already exists!");
    }

    ~ConVar() override;

    [[nodiscard]] ConVarType getType() const;

    [[nodiscard]] std::string_view getTypeAsString() const;

    template<ConVarValidType T>
    [[nodiscard]] inline T getValue() const {
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

    void setValue(ConVarValidType auto newValue, bool runCallback = true) {
        if (this->hasFlag(CON_FLAG_CHEAT) && !ConVar::areCheatsEnabled()) {
            LOG_CONENTRY.error("Cannot set value of cheat-protected ConVar with cheats disabled.");
            return;
        }

        if constexpr (std::is_same_v<decltype(newValue), std::string>) {
            switch (this->type) {
                using enum ConVarType;
                case BOOLEAN:
                    if (newValue == "true") {
                        this->value = "1";
                        break;
                    } else if (newValue == "false") {
                        this->value = "0";
                    }
                    [[fallthrough]];
                case INTEGER:
                    try {
                        this->value = std::to_string(std::stoi(newValue));
                    } catch (const std::invalid_argument&) {
                        this->value = std::to_string(newValue.size());
                    }
                    break;
                case DOUBLE:
                    try {
                        this->value = std::to_string(std::stod(newValue));
                    } catch (const std::invalid_argument&) {
                        this->value = std::to_string(static_cast<double>(newValue.size()));
                    }
                    break;
                case STRING:
                    this->value = newValue;
                    break;
            }
        } else {
            switch (this->type) {
                using enum ConVarType;
                case BOOLEAN:
                    this->value = std::to_string(static_cast<bool>(newValue));
                    break;
                case INTEGER:
                    this->value = std::to_string(static_cast<int>(newValue));
                    break;
                case DOUBLE:
                    this->value = std::to_string(static_cast<double>(newValue));
                    break;
                case STRING:
                    this->value = std::to_string(newValue);
                    break;
            }
        }

        if (runCallback) {
            try {
                this->changedCallback(this->value);
            } catch (const std::exception& e) {
                LOG_CONENTRY.error("Encountered error executing ConVar callback: {}", e.what());
            }
        }
    }

    [[nodiscard]] explicit inline operator std::string() const {
        return std::string{this->getName()} + ": " + this->getTypeAsString().data() + " - " + this->getDescription().data();
    }

    /// Convenience function to check the value of sv_cheats
    [[nodiscard]] static bool areCheatsEnabled();

private:
    std::function<void(CallbackArg)> changedCallback;
    std::string value;
    ConVarType type;
};

class ConCommandRef {
public:
    explicit ConCommandRef(std::string_view name) {
        this->command = ConEntryRegistry::getConCommand(name);
        if (!this->command) {
            LOG_CONENTRY.error("ConCommandRef named \"{}\" refers to a nonexistent ConVar!", name);
        }
    }

    [[nodiscard]] inline std::string_view getName() const { return this->command->getName(); }

    [[nodiscard]] inline std::string_view getDescription() const { return this->command->getDescription(); }

    [[nodiscard]] inline bool hasFlag(ConFlags flag) const { return this->command->hasFlag(flag); }

    inline void fire(ConCommand::CallbackArgs args) { return this->command->fire(args); }

    [[nodiscard]] explicit inline operator std::string() const { return this->command->operator std::string(); }

    [[nodiscard]] explicit inline operator bool() const { return this->command; }

private:
    ConCommand* command;
};

class ConVarRef {
public:
    explicit ConVarRef(std::string_view name) {
        this->var = ConEntryRegistry::getConVar(name);
        if (!this->var) {
            LOG_CONENTRY.error("ConVarRef named \"{}\" refers to a nonexistent ConVar!", name);
        }
    }

    [[nodiscard]] inline std::string_view getName() const { return this->var->getName(); }

    [[nodiscard]] inline std::string_view getDescription() const { return this->var->getDescription(); }

    [[nodiscard]] inline bool hasFlag(ConFlags flag) const { return this->var->hasFlag(flag); }

    [[nodiscard]] inline ConVarType getType() const { return this->var->getType(); }

    [[nodiscard]] inline std::string_view getTypeAsString() const { return this->var->getTypeAsString(); }

    template<ConVarValidType T> [[nodiscard]] inline T getValue() const { return this->var->getValue<T>(); }

    inline void setValue(ConVarValidType auto value, bool runCallback = true) const { return this->var->setValue(value, runCallback); }

    [[nodiscard]] explicit inline operator std::string() const { return this->var->operator std::string(); }

    [[nodiscard]] explicit inline operator bool() const { return this->var; }

private:
    ConVar* var;
};

} // namespace chira
