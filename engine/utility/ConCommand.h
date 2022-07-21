#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <vector>

namespace chira {

class ConCommand {
    using CallbackArgs = std::vector<std::string>;
public:
    ConCommand(std::string name_, std::function<void(const CallbackArgs&)> callback_);
    ConCommand(std::string name_, std::string description_, std::function<void(const CallbackArgs&)> callback_);
    ~ConCommand();
    [[nodiscard]] std::string_view getName() const;
    [[nodiscard]] std::string_view getDescription() const;
    void fire(const CallbackArgs& args);
private:
    std::string name;
    std::string description;
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
