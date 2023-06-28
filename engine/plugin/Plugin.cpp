#include "Plugin.h"

using namespace chira;

namespace {

std::unordered_map<std::string_view, chira::IPlugin*>& getPlugins() {
    static std::unordered_map<std::string_view, IPlugin*> plugins;
    return plugins;
}

std::unordered_map<std::string_view, std::vector<std::string_view>>& getPluginDeps() {
    static std::unordered_map<std::string_view, std::vector<std::string_view>> pluginDeps;
    return pluginDeps;
}

[[nodiscard]] std::vector<IPlugin*>& getPluginOrder() {
    static std::vector<IPlugin*> pluginOrder;
    return pluginOrder;
}

void callAllFns(void(IPlugin::*fn)()) {
    for (IPlugin* plugin : getPluginOrder()) {
        (plugin->*fn)();
    }
}

bool recalculatePluginOrder() {
    DependencyGraph graph;
    std::unordered_map<std::string_view, DependencyGraph::Node*> nodes;
    auto& plugins = getPlugins();
    for (const auto& [name, plugin] : plugins) {
        nodes[name] = graph.addNode(name.data());
    }
    auto& pluginDeps = getPluginDeps();
    for (const auto& [name, plugin] : plugins) {
        for (const auto& dep : pluginDeps.at(name)) {
            nodes[name]->addEdge(nodes[dep]);
        }
    }
    auto order = graph.resolveDependencyOrder();
    if (!order) {
        return false;
    }

    auto& pluginOrder = getPluginOrder();
    pluginOrder.clear();
    for (const auto* node : *order) {
        pluginOrder.push_back(plugins.at(node->name));
    }
    return true;
}

} // namespace

void PluginRegistry::addPlugin(IPlugin* instance, std::string_view name, const std::vector<std::string_view>& deps) {
    auto& plugins = getPlugins();
    auto& pluginDeps = getPluginDeps();
    runtime_assert(!plugins.contains(name), "Plugin already registered!");
    plugins[name] = instance;
    pluginDeps[name] = deps;
}

bool PluginRegistry::preinitAll() {
    if (!recalculatePluginOrder()) {
        return false;
    }
    callAllFns(&IPlugin::preinit);
    return true;
}

void PluginRegistry::initAll() {
    callAllFns(&IPlugin::init);
}

void PluginRegistry::updateAll() {
    callAllFns(&IPlugin::update);
}

void PluginRegistry::renderAll() {
    callAllFns(&IPlugin::render);
}

void PluginRegistry::deinitAll() {
    callAllFns(&IPlugin::deinit);
}
