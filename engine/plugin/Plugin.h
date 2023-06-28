#pragma once

#include <concepts>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <core/Assertions.h>
#include <utility/DependencyGraph.h>

namespace chira {

struct IPlugin {
    virtual void preinit() {};
    virtual void init() {};
    virtual void update() {};
    virtual void render() {};
    virtual void deinit() {};

    [[nodiscard]] bool isInitialized() const {
        return this->initialized;
    }

protected:
    bool initialized = false;
};

} // namespace chira

namespace chira {

template<typename T>
concept CPlugin = std::derived_from<T, IPlugin> && requires(T) {
    {T::DEPS} -> std::same_as<const std::vector<std::string_view>&>;
};

namespace PluginRegistry {

void addPlugin(IPlugin* instance, std::string_view name, const std::vector<std::string_view>& deps);

template<CPlugin P>
inline void addPlugin(P* instance, std::string_view name) {
    addPlugin(instance, name, P::DEPS);
}

[[nodiscard]] bool preinitAll();

void initAll();

void updateAll();

void renderAll();

void deinitAll();

} // namespace PluginRegistry

} // namespace chira

#define CHIRA_CREATE_PLUGIN(name) \
    struct name##Plugin final : public IPlugin

#define CHIRA_REGISTER_PLUGIN(name)                                    \
    const auto get##name##PluginSingleton = [] {                       \
        static name##Plugin singleton##name{};                         \
        static bool registered = false;                                \
        if (!registered) {                                             \
            chira::PluginRegistry::addPlugin(&singleton##name, #name); \
            registered = true;                                         \
        }                                                              \
        return &singleton##name;                                       \
    };                                                                 \
    [[maybe_unused]] name##Plugin* g_##name = get##name##PluginSingleton()

#define CHIRA_GET_PLUGIN(name) \
    extern name##Plugin* g_##name
