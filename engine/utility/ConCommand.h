#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <vector>

namespace chira {

enum ConCommandFlags {
    CONCOMMAND_FLAG_NONE   = 1 << 0, // None
    CONCOMMAND_FLAG_CHEAT  = 1 << 1, // Cheat-protected
    CONCOMMAND_FLAG_HIDDEN = 1 << 2, // Doesn't show up in search
};

class ConCommand {
    using CallbackArgs = std::vector<std::string>;
public:
    ConCommand(std::string name_, std::function<void(const CallbackArgs&)> callback_, int flags_ = CONCOMMAND_FLAG_NONE);
    ConCommand(std::string name_, std::string description_, std::function<void(const CallbackArgs&)> callback_, int flags_ = CONCOMMAND_FLAG_NONE);
    ~ConCommand();
    [[nodiscard]] std::string_view getName() const;
    [[nodiscard]] std::string_view getDescription() const;
    [[nodiscard]] bool hasFlag(ConCommandFlags flag) const;
    void fire(const CallbackArgs& args);
private:
    std::string name;
    std::string description;
    int flags;
    std::function<void(const CallbackArgs&)> callback;
};

class ConCommandRegistry {
    friend ConCommand;
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

} // namespace chira
