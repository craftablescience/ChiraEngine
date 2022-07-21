#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <type_traits>
#include <vector>
#include "Logger.h"

namespace chira {

[[nodiscard]] bool areCheatsEnabled();

enum ConVarFlags {
    CONVAR_FLAG_NONE  = 1 << 0,
    CONVAR_FLAG_CHEAT = 1 << 1,
};

enum class ConVarType {
    BOOLEAN,
    INTEGER,
    FLOAT,
};

class ConVar {
    struct ConVarValue {
        union {
            bool  valueBool;
            int   valueInt;
            float valueFloat;
        };
        ConVarType type;

        explicit ConVarValue(bool  value) : valueBool(value),  type(ConVarType::BOOLEAN) {}
        explicit ConVarValue(int   value) : valueInt(value),   type(ConVarType::INTEGER) {}
        explicit ConVarValue(float value) : valueFloat(value), type(ConVarType::FLOAT)   {}
    };
public:
    ConVar(std::string name_, bool  defaultValue, int flags_ = CONVAR_FLAG_NONE, std::function<void(bool)>  onChanged = [](bool)  {});
    ConVar(std::string name_, int   defaultValue, int flags_ = CONVAR_FLAG_NONE, std::function<void(int)>   onChanged = [](int)   {});
    ConVar(std::string name_, float defaultValue, int flags_ = CONVAR_FLAG_NONE, std::function<void(float)> onChanged = [](float) {});
    ConVar(std::string name_, bool  defaultValue, std::string description_, int flags_ = CONVAR_FLAG_NONE, std::function<void(bool)>  onChanged = [](bool)  {});
    ConVar(std::string name_, int   defaultValue, std::string description_, int flags_ = CONVAR_FLAG_NONE, std::function<void(int)>   onChanged = [](int)   {});
    ConVar(std::string name_, float defaultValue, std::string description_, int flags_ = CONVAR_FLAG_NONE, std::function<void(float)> onChanged = [](float) {});
    ~ConVar();

    [[nodiscard]] ConVarType getType() const;
    [[nodiscard]] std::string_view getName() const;
    [[nodiscard]] std::string_view getDescription() const;
    [[nodiscard]] bool hasFlag(ConVarFlags flag) const;

    template<typename T>
    requires std::is_same_v<T, bool> || std::is_same_v<T, int> || std::is_same_v<T, float>
    inline T getValue() const {
        if constexpr(std::is_same_v<T, bool>) {
            switch (this->value.type) {
                case ConVarType::BOOLEAN:
                    return this->value.valueBool;
                case ConVarType::INTEGER:
                    return static_cast<bool>(this->value.valueInt);
                case ConVarType::FLOAT:
                    return static_cast<bool>(this->value.valueFloat);
            }
        } else if constexpr(std::is_same_v<T, int>) {
            switch (this->value.type) {
                case ConVarType::BOOLEAN:
                    return static_cast<int>(this->value.valueBool);
                case ConVarType::INTEGER:
                    return this->value.valueInt;
                case ConVarType::FLOAT:
                    return static_cast<int>(this->value.valueFloat);
            }
        } else /* if constexpr(std::is_same_v<T, float>) */ {
            switch (this->value.type) {
                case ConVarType::BOOLEAN:
                    return static_cast<float>(this->value.valueBool);
                case ConVarType::INTEGER:
                    return static_cast<float>(this->value.valueInt);
                case ConVarType::FLOAT:
                    return this->value.valueFloat;
            }
        }
        return static_cast<T>(0);
    }

    template<typename T>
    requires std::is_same_v<T, bool> || std::is_same_v<T, int> || std::is_same_v<T, float>
    void setValue(T newValue) {
        if (this->hasFlag(CONVAR_FLAG_CHEAT) && !areCheatsEnabled()) {
            Logger::log(LogType::LOG_ERROR, "ConVar", "Cannot set value of cheat-protected convar with cheats disabled.");
            return;
        }
        switch (this->value.type) {
            case ConVarType::BOOLEAN: {
                bool convertedValue = static_cast<T>(newValue);
                this->value.valueBool = convertedValue;
                this->changedCallbackBool(convertedValue);
                return;
            }
            case ConVarType::INTEGER: {
                int convertedValue = static_cast<T>(newValue);
                this->value.valueInt = convertedValue;
                this->changedCallbackInt(convertedValue);
                return;
            }
            case ConVarType::FLOAT: {
                float convertedValue = static_cast<T>(newValue);
                this->value.valueFloat = convertedValue;
                this->changedCallbackFloat(convertedValue);
                return;
            }
        }
    }

    explicit operator std::string() const;
private:
    std::string name;
    ConVarValue value;
    std::string description;
    int flags;

    std::function<void(bool)>  changedCallbackBool  = [](bool)  {};
    std::function<void(int)>   changedCallbackInt   = [](int)   {};
    std::function<void(float)> changedCallbackFloat = [](float) {};
};

class ConVarReference {
public:
    explicit ConVarReference(std::string name_);
    ConVar& operator*() const;
    ConVar* operator->() const;
    [[nodiscard]] ConVar* get() const;
    [[nodiscard]] bool isValid() const;
private:
    std::string name;
};

class ConVarRegistry {
    friend ConVar;
    friend ConVarReference;
public:
    ConVarRegistry() = delete;
    [[nodiscard]] static bool hasConVar(std::string_view name);
    [[nodiscard]] static std::vector<std::string> getConVarList();
private:
    static std::vector<ConVar*>& getConVars();
    static bool registerConVar(ConVar* convar);
    static void deregisterConVar(ConVar* convar);
    [[nodiscard]] static ConVar* getConVar(std::string_view name);
};

} // namespace chira
