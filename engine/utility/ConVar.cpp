#include "ConVar.h"

#include <algorithm>
#include <utility>
#include "Assertions.h"

using namespace chira;

// Create cheats convar
[[maybe_unused]]
static ConVar cheats{"cheats", false, "Unlocks certain commands that break gameplay."}; // NOLINT(cert-err58-cpp)

namespace chira {

bool areCheatsEnabled() {
    return cheats.getValue<bool>();
}

} // namespace chira

static constexpr const char* NO_DESCRIPTION = "No description provided.";
static constexpr const char* CONVAR_ALREADY_EXISTS = "This convar already exists, use a ConVarReference! This will cause problems...";

ConVar::ConVar(std::string name_, bool defaultValue, int flags_, std::function<void(bool)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(NO_DESCRIPTION)
    , flags(flags_)
    , changedCallbackBool(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, int defaultValue, int flags_, std::function<void(int)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(NO_DESCRIPTION)
    , flags(flags_)
    , changedCallbackInt(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, float defaultValue, int flags_, std::function<void(float)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(NO_DESCRIPTION)
    , flags(flags_)
    , changedCallbackFloat(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, bool defaultValue, std::string description_, int flags_, std::function<void(bool)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(std::move(description_))
    , flags(flags_)
    , changedCallbackBool(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, int defaultValue, std::string description_, int flags_, std::function<void(int)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(std::move(description_))
    , flags(flags_)
    , changedCallbackInt(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, float defaultValue, std::string description_, int flags_, std::function<void(float)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(std::move(description_))
    , flags(flags_)
    , changedCallbackFloat(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::~ConVar() {
    ConVarRegistry::deregisterConVar(this);
}

ConVarType ConVar::getType() const {
    return this->value.type;
}

std::string_view ConVar::getName() const {
    return this->name;
}

std::string_view ConVar::getDescription() const {
    return this->description;
}

bool ConVar::hasFlag(ConVarFlags flag) const {
    return (this->flags & static_cast<int>(flag)) == static_cast<int>(flag);
}

ConVar::operator std::string() const {
    std::string out{this->getName()};
    out += ": ";
    switch (this->value.type) {
        case ConVarType::BOOLEAN:
            out += "boolean";
            break;
        case ConVarType::INTEGER:
            out += "integer";
            break;
        case ConVarType::FLOAT:
            out += "float";
            break;
    }
    return out;
}

ConVarReference::ConVarReference(std::string name_) : name(std::move(name_)) {}

ConVar& ConVarReference::operator*() const {
    return *ConVarRegistry::getConVar(this->name);
}

ConVar* ConVarReference::operator->() const {
    return ConVarRegistry::getConVar(this->name);
}

ConVar* ConVarReference::get() const {
    return ConVarRegistry::getConVar(this->name);
}

bool ConVarReference::isValid() const {
    return ConVarRegistry::hasConVar(this->name);
}

bool ConVarRegistry::hasConVar(std::string_view name) {
    for (const auto* convar : ConVarRegistry::getConVars()) {
        if (convar->getName() == name) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> ConVarRegistry::getConVarList() {
    std::vector<std::string> out;
    for (const auto* convar : ConVarRegistry::getConVars()) {
        out.emplace_back(convar->getName().data());
    }
    return out;
}

std::vector<ConVar*>& ConVarRegistry::getConVars() {
    static std::vector<ConVar*> convars;
    return convars;
}

JSONSettingsLoader& ConVarRegistry::getConVarCache() {
    static JSONSettingsLoader convarCache{"cache.json"};
    return convarCache;
}

bool ConVarRegistry::registerConVar(ConVar* convar) {
    if (!ConVarRegistry::hasConVar(convar->getName())) {
        ConVarRegistry::getConVars().push_back(convar);
        if (convar->hasFlag(CONVAR_FLAG_CACHE) && ConVarRegistry::getConVarCache().hasValue(convar->getName().data())) {
            // There's an entry for the convar in cache, load it
            switch (convar->getType()) {
                case ConVarType::BOOLEAN: {
                    bool value = convar->getValue<bool>();
                    ConVarRegistry::getConVarCache().getValue(convar->getName().data(), &value);
                    convar->setValue(value);
                    break;
                }
                case ConVarType::INTEGER: {
                    int value = convar->getValue<int>();
                    ConVarRegistry::getConVarCache().getValue(convar->getName().data(), &value);
                    convar->setValue(value);
                    break;
                }
                case ConVarType::FLOAT: {
                    // json settings loader works in doubles
                    double value = convar->getValue<float>();
                    ConVarRegistry::getConVarCache().getValue(convar->getName().data(), &value);
                    convar->setValue(static_cast<float>(value));
                    break;
                }
            }
        }
        return true;
    }
    return false;
}

void ConVarRegistry::deregisterConVar(ConVar* convar) {
    // Cache it!
    if (convar->hasFlag(CONVAR_FLAG_CACHE)) {
        switch (convar->getType()) {
            case ConVarType::BOOLEAN:
                ConVarRegistry::getConVarCache().setValue(convar->getName().data(), convar->getValue<bool>(), true, true);
                break;
            case ConVarType::INTEGER:
                ConVarRegistry::getConVarCache().setValue(convar->getName().data(), convar->getValue<int>(), true, true);
                break;
            case ConVarType::FLOAT:
                ConVarRegistry::getConVarCache().setValue(convar->getName().data(), convar->getValue<float>(), true, true);
                break;
        }
    }

    // Erase it!
    ConVarRegistry::getConVars().erase(std::remove_if(ConVarRegistry::getConVars().begin(), ConVarRegistry::getConVars().end(), [convar](ConVar* other) {
        return convar->getName() == other->getName();
    }), ConVarRegistry::getConVars().end());
}

ConVar* ConVarRegistry::getConVar(std::string_view name) {
    for (auto* convar : ConVarRegistry::getConVars()) {
        if (convar->getName() == name) {
            return convar;
        }
    }
    return nullptr;
}
