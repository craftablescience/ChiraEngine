#pragma once

#include <concepts>
#include <string_view>
#include <unordered_map>
#include <utility>

#include <core/Assertions.h>
#include <utility/DependencyGraph.h>

namespace chira {

struct IModule {
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

template<typename T>
concept CModule = std::derived_from<T, IModule> && requires(T) {
    {T::DEPS} -> std::same_as<const std::vector<std::string_view>&>;
};

namespace ModuleRegistry {

void addModule(IModule* instance, std::string_view name, const std::vector<std::string_view>& deps);

template<CModule M>
inline void addModule(M* instance, std::string_view name) {
    addModule(instance, name, M::DEPS);
}

[[nodiscard]] bool preinitAll();

void initAll();

void updateAll();

void renderAll();

void deinitAll();

} // namespace ModuleRegistry

} // namespace chira

#define CHIRA_CREATE_MODULE(name) \
    struct name##Module final : public IModule

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
