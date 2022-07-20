#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <vector>

namespace chira {

class ConVar {
public:
    ConVar(std::string name_, float defaultValue, std::string description_ = "", std::function<void(float)> onChanged = [](float) {});
    ~ConVar();
    [[nodiscard]] float getValue() const;
    void setValue(float newValue);
    [[nodiscard]] std::string_view getName() const;
    [[nodiscard]] std::string_view getDescription() const;
private:
    float value;
    std::string name;
    std::string description;
    std::function<void(float)> changedCallback;
};

class ConVarReference {
public:
    explicit ConVarReference(std::string name_);
    ConVar& operator*() const;
    ConVar* operator->() const;
    [[nodiscard]] ConVar* get() const;
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
    static inline std::vector<ConVar*> convars;
    static void registerConVar(ConVar* convar);
    static void deregisterConVar(ConVar* convar);
    [[nodiscard]] static ConVar* getConVar(std::string_view name);
};

} // namespace chira
