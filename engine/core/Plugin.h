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

template<typename T>
struct Plugin {
    Plugin() = delete;

private:
    [[maybe_unused]] static inline const auto registerFns = [] {
        if constexpr (requires {T::preinit();}) {
            PluginRegistry::addPreinitFn(&T::preinit);
        }
        if constexpr (requires {T::init();}) {
            PluginRegistry::addInitFn(&T::init);
        }
        if constexpr (requires {T::update();}) {
            PluginRegistry::addUpdateFn(&T::update);
        }
        if constexpr (requires {T::deinit();}) {
            PluginRegistry::addDeinitFn(&T::deinit);
        }
        return true;
    } ();
};

#define CHIRA_CREATE_PLUGIN(name) struct name##Plugin : public Plugin<name##Plugin>
#define CHIRA_PLUGIN_PREINIT() static void preinit()
#define CHIRA_PLUGIN_INIT()    static void init()
#define CHIRA_PLUGIN_UPDATE()  static void update()
#define CHIRA_PLUGIN_DEINIT()  static void deinit()
