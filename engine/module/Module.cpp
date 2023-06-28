#include "Module.h"

#include <config/ConEntry.h>

using namespace chira;

CHIRA_CREATE_LOG(MODULE);

namespace {

std::unordered_map<std::string_view, chira::IModule*>& getModules() {
    static std::unordered_map<std::string_view, IModule*> modules;
    return modules;
}

std::unordered_map<std::string_view, std::vector<std::string_view>>& getModuleDeps() {
    static std::unordered_map<std::string_view, std::vector<std::string_view>> moduleDeps;
    return moduleDeps;
}

[[nodiscard]] std::vector<IModule*>& getModuleOrder() {
    static std::vector<IModule*> moduleOrder;
    return moduleOrder;
}

void callAllFns(void(IModule::*fn)()) {
    for (IModule* mod : getModuleOrder()) {
        (mod->*fn)();
    }
}

bool recalculateModuleOrder() {
    DependencyGraph graph;
    std::unordered_map<std::string_view, DependencyGraph::Node*> nodes;
    auto& modules = getModules();
    for (const auto& [name, mod] : modules) {
        nodes[name] = graph.addNode(name.data());
    }
    auto& moduleDeps = getModuleDeps();
    for (const auto& [name, mod] : modules) {
        for (const auto& dep : moduleDeps.at(name)) {
            nodes[name]->addEdge(nodes[dep]);
        }
    }
    auto order = graph.resolveDependencyOrder();
    if (!order) {
        return false;
    }

    auto& moduleOrder = getModuleOrder();
    moduleOrder.clear();
    for (const auto* node : *order) {
        moduleOrder.push_back(modules.at(node->name));
    }
    return true;
}

} // namespace

[[maybe_unused]]
ConCommand list_loaded_modules{"list_loaded_modules", "List all loaded modules.", [](ConCommand::CallbackArgs) {
    for (const auto& [name, mod] : getModules()) {
        LOG_MODULE.infoImportant(name);
    }
}};

void ModuleRegistry::addModule(IModule* instance, std::string_view name, const std::vector<std::string_view>& deps) {
    auto& modules = getModules();
    auto& moduleDeps = getModuleDeps();
    runtime_assert(!modules.contains(name), "Module already registered!");
    modules[name] = instance;
    moduleDeps[name] = deps;
}

bool ModuleRegistry::preinitAll() {
    if (!recalculateModuleOrder()) {
        return false;
    }
    callAllFns(&IModule::preinit);
    return true;
}

void ModuleRegistry::initAll() {
    callAllFns(&IModule::init);
}

void ModuleRegistry::updateAll() {
    callAllFns(&IModule::update);
}

void ModuleRegistry::renderAll() {
    callAllFns(&IModule::render);
}

void ModuleRegistry::deinitAll() {
    callAllFns(&IModule::deinit);
}
