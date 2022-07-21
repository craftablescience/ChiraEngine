#include "ConCommand.h"

#include <algorithm>
#include <utility>
#include "Assertions.h"
#include "ConVar.h"

using namespace chira;

static constexpr const char* NO_DESCRIPTION = "No description provided.";
static constexpr const char* CONCOMMAND_ALREADY_EXISTS = "This concommand already exists! This will cause problems...";

ConCommand::ConCommand(std::string name_, std::function<void(const ConCommand::CallbackArgs&)> callback_, int flags_)
    : name(std::move(name_))
    , description(NO_DESCRIPTION)
    , flags(flags_)
    , callback(std::move(callback_)) {
    runtime_assert(ConCommandRegistry::registerConCommand(this), CONCOMMAND_ALREADY_EXISTS);
}

ConCommand::ConCommand(std::string name_, std::string description_, std::function<void(const ConCommand::CallbackArgs&)> callback_, int flags_)
    : name(std::move(name_))
    , description(std::move(description_))
    , flags(flags_)
    , callback(std::move(callback_)) {
    runtime_assert(ConCommandRegistry::registerConCommand(this), CONCOMMAND_ALREADY_EXISTS);
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

bool ConCommand::hasFlag(ConCommandFlags flag) const {
    return (this->flags & static_cast<int>(flag)) == static_cast<int>(flag);
}

void ConCommand::fire(const ConCommand::CallbackArgs& args) {
    if (this->hasFlag(CONCOMMAND_FLAG_CHEAT) && !areCheatsEnabled()) {
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
