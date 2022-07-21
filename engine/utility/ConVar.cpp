#include "ConVar.h"

#include <algorithm>
#include <utility>
#include <utility/Assertions.h>

using namespace chira;

static constexpr const char* NO_DESCRIPTION = "No description provided.";
static constexpr const char* CONVAR_ALREADY_EXISTS = "This convar already exists, use a ConVarReference! This will cause problems...";

ConVar::ConVar(std::string name_, bool defaultValue, std::function<void(bool)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(NO_DESCRIPTION)
    , changedCallbackBool(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, int defaultValue, std::function<void(int)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(NO_DESCRIPTION)
    , changedCallbackInt(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, float defaultValue, std::function<void(float)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(NO_DESCRIPTION)
    , changedCallbackFloat(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, bool defaultValue, std::string description_, std::function<void(bool)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(std::move(description_))
    , changedCallbackBool(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, int defaultValue, std::string description_, std::function<void(int)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(std::move(description_))
    , changedCallbackInt(std::move(onChanged)) {
    runtime_assert(ConVarRegistry::registerConVar(this), CONVAR_ALREADY_EXISTS);
}

ConVar::ConVar(std::string name_, float defaultValue, std::string description_, std::function<void(float)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(std::move(description_))
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

bool ConVarRegistry::registerConVar(ConVar* convar) {
    if (!ConVarRegistry::hasConVar(convar->getName())) {
        ConVarRegistry::getConVars().push_back(convar);
        return true;
    }
    return false;
}

void ConVarRegistry::deregisterConVar(ConVar* convar) {
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
