#pragma once

#include <array>
#include <concepts>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "debug/Assertions.h"
#include "utility/DependencyGraph.h"
#include "utility/MacroHelpers.h"
#include "utility/NoCopyOrMove.h"

namespace chira {

namespace ModuleRegistry {

[[nodiscard]] bool initAll();

void updateAll();

void renderAll();

void deinitAll();

} // namespace ModuleRegistry

struct IModule : public NoCopyOrMove {
    friend bool ModuleRegistry::initAll();

    /**
     * Ran before modules are fully initialized. Module dependencies have been initialized already.
     * @return <code>true</code> if module was successfully initialized, else <code>false</code>
     */
    virtual bool preinit() { return true; };

    /// Ran after all modules are initialized.
    virtual void postinit() {};

    /// Ran on every update call.
    virtual void update() {};

    /// Ran on every render call.
    virtual void render() {};

    /// Called on modules in reverse order at exit. Modules can assume their dependencies still exist.
    virtual void deinit() {};

    /**
     * Check if the module has been initialized.
     * @return <code>true</code> if preinit has been called on the given module, else <code>false</code>
     */
    [[nodiscard]] bool isInitialized() const {
        return this->initialized;
    }

protected:
    bool initialized = false;
};

/**
 * Determines if the given type is a module. Modules need to define a public static variable
 * <code>std::array<std::string_view> DEPS</code>
 * @tparam T potential module
 */
template<typename T>
concept CModule = std::derived_from<T, IModule> && std::same_as<typename decltype(T::DEPS)::value_type, std::string_view> &&
requires(T) {
    std::begin(T::DEPS);
    std::end(T::DEPS);
};

namespace ModuleRegistry {

void addModule(IModule* instance, std::string_view name, std::vector<std::string_view>&& deps);

template<CModule M>
inline void addModule(M* instance, std::string_view name) {
    addModule(instance, name, {std::begin(M::DEPS), std::end(M::DEPS)});
}

} // namespace ModuleRegistry

} // namespace chira

#define CHIRA_CREATE_MODULE(name) \
    struct name##Module final : public IModule

#define CHIRA_CREATE_MODULE_DEPS(...) \
    static constexpr std::array<std::string_view, CHIRA_ARG_COUNT(__VA_ARGS__)> DEPS {__VA_ARGS__}

#define CHIRA_REGISTER_MODULE(name)                                    \
    const auto get##name##ModuleSingleton = [] {                       \
        static name##Module singleton##name{};                         \
        static bool registered = false;                                \
        if (!registered) {                                             \
            chira::ModuleRegistry::addModule(&singleton##name, #name); \
            registered = true;                                         \
        }                                                              \
        return &singleton##name;                                       \
    };                                                                 \
    [[maybe_unused]] name##Module* g_##name = get##name##ModuleSingleton()

#define CHIRA_GET_MODULE(name) \
    extern name##Module* g_##name
