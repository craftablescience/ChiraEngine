#include <gtest/gtest.h>
#include <TestHelpers.h>

#include <core/math/Graph.h>

using namespace chira;

TEST(UnweightedDirectedGraph, addAndRemoveNodes) {
    UnweightedDirectedGraph graph;

    auto* a = graph.addNode("a");
    graph.addNode("b");
    EXPECT_EQ(graph.getNodes().size(), 2);

    graph.removeNode(&a);
    EXPECT_EQ(graph.getNodes().size(), 1);
    EXPECT_EQ(a, nullptr);
}
