#pragma once

#include <type_traits>
#include <vector>

struct IPlugin {
    virtual void preinit() {};
    virtual void init() {};
    virtual void update() {};
    virtual void deinit() {};
};

/// Systems are sections of the engine that are largely self-contained. They are stored in whatever order
/// the compiler decides to create them in, so entries should be treated as unordered!
class PluginRegistry {
public:
    PluginRegistry() = delete;

    template<typename P>
    requires std::is_base_of_v<IPlugin, P>
    static void addPlugin(P* instance) {
        PluginRegistry::getPlugins().push_back(instance);
    }

    static void preinitAll() {
        PluginRegistry::callAllFns(&IPlugin::preinit);
    }
    static void initAll() {
        PluginRegistry::callAllFns(&IPlugin::init);
    }
    static void updateAll() {
        PluginRegistry::callAllFns(&IPlugin::update);
    }
    static void deinitAll() {
        PluginRegistry::callAllFns(&IPlugin::deinit);
    }

private:
    static std::vector<IPlugin*>& getPlugins() {
        static std::vector<IPlugin*> plugins;
        return plugins;
    }

    static void callAllFns(void(IPlugin::*fn)()) {
        for (auto plugin : PluginRegistry::getPlugins()) {
            (plugin->*fn)();
        }
    }
};

#define CHIRA_REGISTER_PLUGIN(clazz) \
    [[maybe_unused]] const auto clazz##_RegisteredSingleton_Registerer = [] { \
        static clazz clazz##_RegisteredSingleton{}; \
        PluginRegistry::addPlugin(&clazz##_RegisteredSingleton); \
        return true; \
    } ()
