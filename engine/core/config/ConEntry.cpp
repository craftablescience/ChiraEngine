#include "ConEntry.h"

#include <algorithm>
#include <utility>

using namespace chira;

CHIRA_CREATE_LOG(CONENTRY);

[[maybe_unused]]
ConCommand about{"about", "Prints the description of the given convar(s) or concommand(s).", [](ConCommand::CallbackArgs args) {
    for (const auto& name : args) {
        if (ConEntryRegistry::hasConCommand(name)) {
            LOG_CONENTRY.infoImportant(std::string{*ConEntryRegistry::getConCommand(name)});
        } else if (ConEntryRegistry::hasConVar(name)) {
            LOG_CONENTRY.infoImportant(std::string{*ConEntryRegistry::getConVar(name)});
        } else {
            LOG_CONENTRY.infoImportant("Unknown console entry \"{}\"", name);
        }
    }
}};

[[maybe_unused]]
ConCommand find{"find", "Finds convars and/or concommands from the given substring.", [](ConCommand::CallbackArgs args) {
    bool resultFound = false;
    for (const auto& substr : args) {
        for (const auto& concommand: ConEntryRegistry::getConCommandList()) {
            if (concommand.find(substr) != std::string::npos) {
                LOG_CONENTRY.infoImportant(std::string{*ConEntryRegistry::getConCommand(concommand)});
                resultFound = true;
            }
        }
        for (const auto& convar: ConEntryRegistry::getConVarList()) {
            if (convar.find(substr) != std::string::npos) {
                LOG_CONENTRY.infoImportant(std::string{*ConEntryRegistry::getConVar(convar)});
                resultFound = true;
            }
        }
    }
    if (!resultFound) {
        LOG_CONENTRY.infoImportant("No results.");
    }
}};

[[maybe_unused]]
ConCommand con_entries{"con_entries", "Prints the description of every convar and concommand currently registered.", [] {
    LOG_CONENTRY.infoImportant("-- Commands --");
    auto concommandList = ConEntryRegistry::getConCommandList();
    std::sort(concommandList.begin(), concommandList.end());
    for (const auto& name : concommandList) {
        if (const auto* concommand = ConEntryRegistry::getConCommand(name); !concommand->hasFlag(CON_FLAG_HIDDEN)) {
            LOG_CONENTRY.infoImportant(std::string{*concommand});
        }
    }

    LOG_CONENTRY.infoImportant("-- Variables --");
    auto convarList = ConEntryRegistry::getConVarList();
    std::sort(convarList.begin(), convarList.end());
    for (const auto& name : convarList) {
        if (const auto* convar = ConEntryRegistry::getConVar(name); !convar->hasFlag(CON_FLAG_HIDDEN)) {
            LOG_CONENTRY.infoImportant(std::string{*convar});
        }
    }
}};

ConEntry::ConEntry(std::string name_, std::string description_, int flags_)
        : name(std::move(name_))
        , description(std::move(description_))
        , flags(flags_) {}

std::string_view ConEntry::getName() const {
    return this->name;
}

std::string_view ConEntry::getDescription() const {
    return this->description;
}

bool ConEntry::hasFlag(ConFlags flag) const {
    return this->flags & flag;
}

ConCommand::ConCommand(std::string name_, const std::function<void()>& callback_, int flags_)
    : ConCommand(std::move(name_), "No description provided.", callback_, flags_) {}

ConCommand::ConCommand(std::string name_, std::function<void(ConCommand::CallbackArgs)> callback_, int flags_)
    : ConCommand(std::move(name_), "No description provided.", std::move(callback_), flags_) {}

ConCommand::ConCommand(std::string name_, std::string description_, const std::function<void()>& callback_, int flags_)
    : ConCommand(std::move(name_), std::move(description_), [callback_](ConCommand::CallbackArgs) {callback_();}, flags_) {}

ConCommand::ConCommand(std::string name_, std::string description_, std::function<void(ConCommand::CallbackArgs)> callback_, int flags_)
    : ConEntry(std::move(name_), std::move(description_), flags_)
    , callback(std::move(callback_)) {
    chiraAssert(ConEntryRegistry::registerConCommand(this), "This ConCommand already exists!");
}

ConCommand::~ConCommand() {
    ConEntryRegistry::deregisterConCommand(this);
}

void ConCommand::fire(ConCommand::CallbackArgs args) {
    if (this->hasFlag(CON_FLAG_CHEAT) && !ConVar::areCheatsEnabled()) {
        LOG_CONENTRY.error("Cannot fire cheat-protected ConCommand with cheats disabled.");
        return;
    }
    this->callback(args);
}

bool ConEntryRegistry::hasConCommand(std::string_view name) {
    for (const auto* concommand : ConEntryRegistry::getConCommands()) {
        if (concommand->getName() == name) {
            return true;
        }
    }
    return false;
}

ConCommand* ConEntryRegistry::getConCommand(std::string_view name) {
    for (auto* concommand : ConEntryRegistry::getConCommands()) {
        if (concommand->getName() == name) {
            return concommand;
        }
    }
    return nullptr;
}

std::vector<std::string> ConEntryRegistry::getConCommandList() {
    std::vector<std::string> out;
    for (const auto* concommand : ConEntryRegistry::getConCommands()) {
        out.emplace_back(concommand->getName().data());
    }
    return out;
}

std::vector<ConCommand*>& ConEntryRegistry::getConCommands() {
    static std::vector<ConCommand*> concommands;
    return concommands;
}

bool ConEntryRegistry::registerConCommand(ConCommand* concommand) {
    if (ConEntryRegistry::hasConCommand(concommand->getName())) {
	    return false;
    }
    ConEntryRegistry::getConCommands().push_back(concommand);
    return true;
}

void ConEntryRegistry::deregisterConCommand(ConCommand* concommand) {
    auto& concommands = ConEntryRegistry::getConCommands();
    concommands.erase(std::remove_if(concommands.begin(), concommands.end(), [concommand](ConCommand* other) {
        return concommand->getName() == other->getName();
    }), concommands.end());
}

bool ConEntryRegistry::hasConVar(std::string_view name) {
    for (const auto* convar : ConEntryRegistry::getConVars()) {
        if (convar->getName() == name) {
            return true;
        }
    }
    return false;
}

ConVar* ConEntryRegistry::getConVar(std::string_view name) {
    for (auto* convar : ConEntryRegistry::getConVars()) {
        if (convar->getName() == name) {
            return convar;
        }
    }
    return nullptr;
}

std::vector<std::string> ConEntryRegistry::getConVarList() {
    std::vector<std::string> out;
    for (const auto* convar : ConEntryRegistry::getConVars()) {
        out.emplace_back(convar->getName().data());
    }
    return out;
}

void ConEntryRegistry::initializeConVarCache(ConVar* initializeOnlyMe) {
	const auto loadCachedValue = [](ConVar* convar) {
		if (!convar->hasFlag(CON_FLAG_CACHE)) {
			return;
		}
		// There's an entry for the convar in cache, load it
		switch (convar->getType()) {
			using enum ConVarType;
			case BOOLEAN: {
				auto value = convar->getValue<bool>();
				ConEntryRegistry::getConVarCache().getValue(convar->getName().data(), &value);
				convar->setValue(value, false);
				break;
			}
			case INTEGER: {
				auto value = convar->getValue<int>();
				ConEntryRegistry::getConVarCache().getValue(convar->getName().data(), &value);
				convar->setValue(value, false);
				break;
			}
			case DOUBLE: {
				auto value = convar->getValue<double>();
				ConEntryRegistry::getConVarCache().getValue(convar->getName().data(), &value);
				convar->setValue(value, false);
				break;
			}
			case STRING: {
				auto value = convar->getValue<std::string>();
				ConEntryRegistry::getConVarCache().getValue(convar->getName().data(), &value);
				convar->setValue(value, false);
				break;
			}
		}
	};
	if (initializeOnlyMe) {
		// We've already initialized the main cache, initialize this convar
		loadCachedValue(initializeOnlyMe);
	} else {
		if (!ConEntryRegistry::getConVarCache().isOpen()) {
			ConEntryRegistry::getConVarCache().open("convars.json");
		}
		for (auto* convar : ConEntryRegistry::getConVars()) {
			loadCachedValue(convar);
		}
	}
}

std::vector<ConVar*>& ConEntryRegistry::getConVars() {
    static std::vector<ConVar*> convars;
    return convars;
}

IConfigFile& ConEntryRegistry::getConVarCache() {
    static JSONConfigFile conVarCache{};
    return conVarCache;
}

bool ConEntryRegistry::registerConVar(ConVar* convar) {
    if (ConEntryRegistry::hasConVar(convar->getName())) {
	    return false;
    }
    ConEntryRegistry::getConVars().push_back(convar);
	if (ConEntryRegistry::getConVarCache().isOpen()) {
		// Need to manually initialize this convar from cache, it didn't get picked up earlier
		ConEntryRegistry::initializeConVarCache(convar);
	}
    return true;
}

void ConEntryRegistry::deregisterConVar(ConVar* convar) {
    // Cache it!
    if (convar->hasFlag(CON_FLAG_CACHE)) {
        auto& cache = ConEntryRegistry::getConVarCache();
        switch (convar->getType()) {
            using enum ConVarType;
            case BOOLEAN:
                cache.setValue(convar->getName().data(), convar->getValue<bool>(), true, true);
                break;
            case INTEGER:
                cache.setValue(convar->getName().data(), convar->getValue<int>(), true, true);
                break;
            case DOUBLE:
                cache.setValue(convar->getName().data(), convar->getValue<double>(), true, true);
                break;
            case STRING:
                cache.setValue(convar->getName().data(), convar->getValue<std::string>(), true, true);
        }
    }

    // Erase it!
    auto& convars = ConEntryRegistry::getConVars();
    convars.erase(std::remove_if(convars.begin(), convars.end(), [convar](ConVar* other) {
        return convar->getName() == other->getName();
    }), convars.end());
}

ConVar::~ConVar() {
    ConEntryRegistry::deregisterConVar(this);
}

ConVarType ConVar::getType() const {
    return this->type;
}

std::string_view ConVar::getTypeAsString() const {
    switch (this->type) {
        using enum ConVarType;
        case BOOLEAN:
            return "boolean";
        case INTEGER:
            return "integer";
        case DOUBLE:
            return "double";
        case STRING:
            return "string";
    }
    return "";
}

ConVar sv_cheats{"sv_cheats", false, "Unlocks certain console entries that break gameplay."};

bool ConVar::areCheatsEnabled() {
    return sv_cheats.getValue<bool>();
}
