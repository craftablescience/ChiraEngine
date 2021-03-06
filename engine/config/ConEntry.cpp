#include "ConEntry.h"

#include <algorithm>
#include <ranges>
#include <utility>

using namespace chira;

[[maybe_unused]]
static ConCommand info{"info", "Prints the description of the given convar(s) or concommand(s).", [](ConCommand::CallbackArgs args) { // NOLINT(cert-err58-cpp)
    for (const auto& name : args) {
        if (ConCommandRegistry::hasConCommand(name)) {
            Logger::log(LogType::LOG_INFO_IMPORTANT, "Console", std::string{*ConCommandRegistry::getConCommand(name)});
        } else if (ConVarRegistry::hasConVar(name)) {
            Logger::log(LogType::LOG_INFO_IMPORTANT, "Console", std::string{*ConVarRegistry::getConVar(name)});
        }
    }
}};

[[maybe_unused]]
static ConCommand con_entries{"con_entries", "Prints the description of every convar and concommand currently registered.", [] { // NOLINT(cert-err58-cpp)
    Logger::log(LogType::LOG_INFO_IMPORTANT, "Console", "-- Commands --");
    auto concommandList = ConCommandRegistry::getConCommandList();
    std::ranges::sort(concommandList);
    for (const auto& name : concommandList) {
        if (const auto* concommand = ConCommandRegistry::getConCommand(name); !concommand->hasFlag(CON_FLAG_HIDDEN)) {
            Logger::log(LogType::LOG_INFO_IMPORTANT, "Console", std::string{*concommand});
        }
    }

    Logger::log(LogType::LOG_INFO_IMPORTANT, "Console", "-- Variables --");
    auto convarList = ConVarRegistry::getConVarList();
    std::ranges::sort(convarList);
    for (const auto& name : convarList) {
        if (const auto* convar = ConVarRegistry::getConVar(name); !convar->hasFlag(CON_FLAG_HIDDEN)) {
            Logger::log(LogType::LOG_INFO_IMPORTANT, "Console", std::string{*convar});
        }
    }
}};

ConCommand::ConCommand(std::string name_, const std::function<void()>& callback_, int flags_)
    : ConCommand(std::move(name_), "No description provided.", callback_, flags_) {}

ConCommand::ConCommand(std::string name_, std::function<void(ConCommand::CallbackArgs)> callback_, int flags_)
    : ConCommand(std::move(name_), "No description provided.", std::move(callback_), flags_) {}

ConCommand::ConCommand(std::string name_, std::string description_, const std::function<void()>& callback_, int flags_)
    : ConCommand(std::move(name_), std::move(description_), [callback_](ConCommand::CallbackArgs) {callback_();}, flags_) {}

ConCommand::ConCommand(std::string name_, std::string description_, std::function<void(ConCommand::CallbackArgs)> callback_, int flags_)
    : name(std::move(name_))
    , description(std::move(description_))
    , flags(flags_)
    , callback(std::move(callback_)) {
    runtime_assert(ConCommandRegistry::registerConCommand(this), "This concommand already exists! This will cause problems...");
}

ConCommand::~ConCommand() {
    ConCommandRegistry::deregisterConCommand(this);
}

std::string_view ConCommand::getName() const {
    return this->name;
}

std::string_view ConCommand::getDescription() const {
    return this->description;
}

bool ConCommand::hasFlag(ConFlags flag) const {
    return (this->flags & static_cast<int>(flag)) == static_cast<int>(flag);
}

void ConCommand::fire(ConCommand::CallbackArgs args) {
    if (this->hasFlag(CON_FLAG_CHEAT) && !ConVar::areCheatsEnabled()) {
        Logger::log(LogType::LOG_ERROR, "ConCommand", "Cannot fire cheat-protected concommand with cheats disabled.");
        return;
    }
    this->callback(args);
}

bool ConCommandRegistry::hasConCommand(std::string_view name) {
    for (const auto* concommand : ConCommandRegistry::getConCommands()) {
        if (concommand->getName() == name) {
            return true;
        }
    }
    return false;
}

ConCommand* ConCommandRegistry::getConCommand(std::string_view name) {
    for (auto* concommand : ConCommandRegistry::getConCommands()) {
        if (concommand->getName() == name) {
            return concommand;
        }
    }
    return nullptr;
}

std::vector<std::string> ConCommandRegistry::getConCommandList() {
    std::vector<std::string> out;
    for (const auto* concommand : ConCommandRegistry::getConCommands()) {
        out.emplace_back(concommand->getName().data());
    }
    return out;
}

std::vector<ConCommand*>& ConCommandRegistry::getConCommands() {
    static std::vector<ConCommand*> concommands;
    return concommands;
}

bool ConCommandRegistry::registerConCommand(ConCommand* concommand) {
    if (!ConCommandRegistry::hasConCommand(concommand->getName())) {
        ConCommandRegistry::getConCommands().push_back(concommand);
        return true;
    }
    return false;
}

void ConCommandRegistry::deregisterConCommand(ConCommand* concommand) {
    ConCommandRegistry::getConCommands().erase(std::remove_if(ConCommandRegistry::getConCommands().begin(), ConCommandRegistry::getConCommands().end(), [concommand](ConCommand* other) {
        return concommand->getName() == other->getName();
    }), ConCommandRegistry::getConCommands().end());
}

// Create cheats convar
[[maybe_unused]]
static ConVar cheats{"cheats", false, "Unlocks certain commands that break gameplay."}; // NOLINT(cert-err58-cpp)

bool ConVar::areCheatsEnabled() {
    return cheats.getValue<bool>();
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
        if (convar->hasFlag(CON_FLAG_CACHE) && ConVarRegistry::getConVarCache().hasValue(convar->getName().data())) {
            // There's an entry for the convar in cache, load it
            switch (convar->getType()) {
                case ConVarType::BOOLEAN: {
                    auto value = convar->getValue<bool>();
                    ConVarRegistry::getConVarCache().getValue(convar->getName().data(), &value);
                    convar->setValue(value, false);
                    break;
                }
                case ConVarType::INTEGER: {
                    auto value = convar->getValue<int>();
                    ConVarRegistry::getConVarCache().getValue(convar->getName().data(), &value);
                    convar->setValue(value, false);
                    break;
                }
                case ConVarType::DOUBLE: {
                    auto value = convar->getValue<double>();
                    ConVarRegistry::getConVarCache().getValue(convar->getName().data(), &value);
                    convar->setValue(value, false);
                    break;
                }
                case ConVarType::STRING: {
                    auto value = convar->getValue<std::string>();
                    ConVarRegistry::getConVarCache().getValue(convar->getName().data(), &value);
                    convar->setValue(value, false);
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
    if (convar->hasFlag(CON_FLAG_CACHE)) {
        switch (convar->getType()) {
            case ConVarType::BOOLEAN:
                ConVarRegistry::getConVarCache().setValue(convar->getName().data(), convar->getValue<bool>(), true, true);
                break;
            case ConVarType::INTEGER:
                ConVarRegistry::getConVarCache().setValue(convar->getName().data(), convar->getValue<int>(), true, true);
                break;
            case ConVarType::DOUBLE:
                ConVarRegistry::getConVarCache().setValue(convar->getName().data(), convar->getValue<double>(), true, true);
                break;
            case ConVarType::STRING:
                ConVarRegistry::getConVarCache().setValue(convar->getName().data(), convar->getValue<std::string>(), true, true);
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
