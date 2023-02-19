#pragma once

#include <vector>

/// Systems are sections of the engine that are largely self-contained. They are stored in whatever order
/// the compiler decides to create them in, so entries should be treated as unordered!
class PluginRegistry {
    template<typename T>
    friend struct Plugin;
public:
    PluginRegistry() = delete;

    static void addPreinitFn(void(*fn)()) {
        PluginRegistry::getPreinitFns().push_back(fn);
    }
    static void addInitFn(void(*fn)()) {
        PluginRegistry::getInitFns().push_back(fn);
    }
    static void addUpdateFn(void(*fn)()) {
        PluginRegistry::getUpdateFns().push_back(fn);
    }
    static void addDeinitFn(void(*fn)()) {
        PluginRegistry::getDeinitFns().push_back(fn);
    }

    static void preinitAll() {
        PluginRegistry::callAllFns(PluginRegistry::getPreinitFns());
    }
    static void initAll() {
        PluginRegistry::callAllFns(PluginRegistry::getInitFns());
    }
    static void updateAll() {
        PluginRegistry::callAllFns(PluginRegistry::getUpdateFns());
    }
    static void deinitAll() {
        PluginRegistry::callAllFns(PluginRegistry::getDeinitFns());
    }

private:
    static std::vector<void(*)()>& getPreinitFns() {
        static std::vector<void(*)()> preinitFns;
        return preinitFns;
    }
    static std::vector<void(*)()>& getInitFns() {
        static std::vector<void(*)()> initFns;
        return initFns;
    }
    static std::vector<void(*)()>& getUpdateFns() {
        static std::vector<void(*)()> updateFns;
        return updateFns;
    }
    static std::vector<void(*)()>& getDeinitFns() {
        static std::vector<void(*)()> deinitFns;
        return deinitFns;
    }

    static void callAllFns(const std::vector<void(*)()>& fns) {
        for (auto fn : fns) {
            (*fn)();
        }
    }
};

#define CHIRA_CREATE_PLUGIN(name) struct name##Plugin
#define CHIRA_REGISTER_PLUGIN(name) \
    [[maybe_unused]] const auto name##Plugin_Register_Funcs = [] { \
        if constexpr (requires {name##Plugin::preinit();}) { \
            PluginRegistry::addPreinitFn(&name##Plugin::preinit); \
        } \
        if constexpr (requires {name##Plugin::init();}) { \
            PluginRegistry::addInitFn(&name##Plugin::init); \
        } \
        if constexpr (requires {name##Plugin::update();}) { \
            PluginRegistry::addUpdateFn(&name##Plugin::update); \
        } \
        if constexpr (requires {name##Plugin::deinit();}) { \
            PluginRegistry::addDeinitFn(&name##Plugin::deinit); \
        } \
        return true; \
    } ()

#define CHIRA_PLUGIN_PREINIT() static void preinit()
#define CHIRA_PLUGIN_INIT()    static void init()
#define CHIRA_PLUGIN_UPDATE()  static void update()
#define CHIRA_PLUGIN_DEINIT()  static void deinit()
