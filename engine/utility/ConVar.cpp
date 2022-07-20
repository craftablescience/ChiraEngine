#include "ConVar.h"

#include <algorithm>
#include <utility>

using namespace chira;

ConVar::ConVar(std::string name_, float defaultValue, std::string description_, std::function<void(float)> onChanged)
    : name(std::move(name_))
    , value(defaultValue)
    , description(std::move(description_))
    , changedCallback(std::move(onChanged)) {
    ConVarRegistry::registerConVar(this);
}

ConVar::~ConVar() {
    ConVarRegistry::deregisterConVar(this);
}

float ConVar::getValue() const {
    return this->value;
}

void ConVar::setValue(float newValue) {
    this->value = newValue;
    this->changedCallback(newValue);
}

std::string_view ConVar::getName() const {
    return this->name;
}

std::string_view ConVar::getDescription() const {
    return this->description;
}

bool ConVarRegistry::hasConVar(std::string_view name) {
    for (const auto* convar : ConVarRegistry::convars) {
        if (convar->getName() == name) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> ConVarRegistry::getConVarList() {
    std::vector<std::string> out;
    for (const auto* convar : ConVarRegistry::convars) {
        out.emplace_back(convar->getName().data());
    }
    return out;
}

void ConVarRegistry::registerConVar(ConVar* convar) {
    if (!ConVarRegistry::hasConVar(convar->getName())) {
        ConVarRegistry::convars.push_back(convar);
    }
}

void ConVarRegistry::deregisterConVar(ConVar* convar) {
    ConVarRegistry::convars.erase(std::remove_if(ConVarRegistry::convars.begin(), ConVarRegistry::convars.end(), [convar](ConVar* other) {
        return convar->getName() == other->getName();
    }), ConVarRegistry::convars.end());
}

ConVar* ConVarRegistry::getConVar(std::string_view name) {
    for (auto* convar : ConVarRegistry::convars) {
        if (convar->getName() == name) {
            return convar;
        }
    }
    return nullptr;
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
