#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace chira {

class UnweightedDirectedGraph {
public:
    struct Node {
    private:
        friend class UnweightedDirectedGraph;
    public:
        void addEdge(Node* node) {
            this->edges.push_back(node);
        }

        void removeEdge(Node* node) {
            this->edges.erase(std::remove(this->edges.begin(), this->edges.end(), node), this->edges.end());
        }

        std::string name;
        std::vector<Node*> edges;

    private:
        explicit Node(std::string name_)
                : name(std::move(name_)) {}
    };

    UnweightedDirectedGraph() = default;

    Node* addNode(const std::string& name) {
        auto node = new Node{name};
        this->nodes.emplace_back(node);
        return node;
    }

    [[nodiscard]] Node* getNode(const std::string& name) const {
        if (auto node = std::find_if(this->nodes.begin(), this->nodes.end(), [name](const std::unique_ptr<Node>& item) {
            return item->name == name;
        }); node != this->nodes.end()) {
            return node->get();
        }
        return nullptr;
    }

    void removeNode(Node** nodeToRemove) {
        for (const auto& node : this->nodes) {
            node->removeEdge(*nodeToRemove);
        }
        this->nodes.erase(std::remove_if(this->nodes.begin(), this->nodes.end(), [nodeToRemove] (const std::unique_ptr<Node>& node) {
            return node.get() == *nodeToRemove;
        }), this->nodes.end());
        *nodeToRemove = nullptr;
    }

    [[nodiscard]] const std::vector<std::unique_ptr<Node>>& getNodes() const {
        return this->nodes;
    }

protected:
    std::vector<std::unique_ptr<Node>> nodes;
};

} // namespace chira
