#pragma once

#include <optional>

#include "../math/Graph.h"

namespace chira {

class DependencyGraph : public UnweightedDirectedGraph {
public:
    DependencyGraph()
            : UnweightedDirectedGraph() {}

    /// Get the order that dependencies should be initialized in, or std::nullopt if there is a cycle
    [[nodiscard]] std::optional<std::vector<const Node*>> resolveDependencyOrder() const {
        std::vector<const Node*> resolved;
        std::vector<const Node*> unresolved;
        for (const auto& node : this->nodes) {
            const auto* edge = node.get();
            if (std::find(resolved.begin(), resolved.end(), edge) != resolved.end()) {
                continue;
            }
            if (std::find(unresolved.begin(), unresolved.end(), edge) != unresolved.end()) {
                return std::nullopt;
            }
            if (!resolveDependencyOrderRecurse(edge, resolved, unresolved)) {
                return std::nullopt;
            }
        }
        return resolved;
    }

    /// Get the order that dependencies should be initialized in, or std::nullopt if there is a cycle
    [[nodiscard]] static std::optional<std::vector<const Node*>> resolveDependencyOrder(const Node* root) {
        std::vector<const Node*> resolved;
        std::vector<const Node*> unresolved;
        if (!resolveDependencyOrderRecurse(root, resolved, unresolved)) {
            return std::nullopt;
        }
        return resolved;
    }

protected:
    // NOLINTNEXTLINE(misc-no-recursion)
    static bool resolveDependencyOrderRecurse(const Node* node, std::vector<const Node*>& resolved, std::vector<const Node*>& unresolved) {
        unresolved.push_back(node);
        for (const auto* edge : node->edges) {
            if (std::find(resolved.begin(), resolved.end(), edge) != resolved.end()) {
                continue;
            }
            if (std::find(unresolved.begin(), unresolved.end(), edge) != unresolved.end()) {
                return false;
            }
            if (!resolveDependencyOrderRecurse(edge, resolved, unresolved)) {
                return false;
            }
        }
        unresolved.erase(std::remove(unresolved.begin(), unresolved.end(), node), unresolved.end());
        resolved.push_back(node);
        return true;
    }
};

} // namespace chira
