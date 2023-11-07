#include <gtest/gtest.h>

#include <core/utility/DependencyGraph.h>

using namespace chira;

/*
 *   A
 *  ↙ ↘
 * B   C
 *
 * Expected: B → C → A
 */
TEST(DependencyGraph, simple) {
    DependencyGraph graph;

    auto* nodeA = graph.addNode("A");
    auto* nodeB = graph.addNode("B");
    auto* nodeC = graph.addNode("C");

    nodeA->addEdge(nodeB);
    nodeA->addEdge(nodeC);

    auto result1 = graph.resolveDependencyOrder();
    ASSERT_TRUE(result1);

    std::string result1str;
    for (const auto* item : *result1) {
        result1str += item->name;
    }
    EXPECT_STREQ(result1str.c_str(), "BCA");

    auto result2 = DependencyGraph::resolveDependencyOrder(nodeA);
    ASSERT_TRUE(result2);

    std::string result2str;
    for (const auto* item : *result2) {
        result2str += item->name;
    }
    EXPECT_STREQ(result2str.c_str(), "BCA");
}

/*
 *   A
 *  ↙ ↘
 * D   B
 *  ↖ ↙ ↘
 *   C   E
 *
 * Expected: D → C → E → B → A
 */
TEST(DependencyGraph, complex) {
    DependencyGraph graph;

    auto* nodeA = graph.addNode("A");
    auto* nodeB = graph.addNode("B");
    auto* nodeC = graph.addNode("C");
    auto* nodeD = graph.addNode("D");
    auto* nodeE = graph.addNode("E");

    nodeA->addEdge(nodeD);
    nodeA->addEdge(nodeB);
    nodeB->addEdge(nodeC);
    nodeB->addEdge(nodeE);
    nodeC->addEdge(nodeD);

    auto result1 = graph.resolveDependencyOrder();
    ASSERT_TRUE(result1);

    std::string result1str;
    for (const auto* item : *result1) {
        result1str += item->name;
    }
    EXPECT_STREQ(result1str.c_str(), "DCEBA");

    auto result2 = DependencyGraph::resolveDependencyOrder(nodeA);
    ASSERT_TRUE(result2);

    std::string result2str;
    for (const auto* item : *result2) {
        result2str += item->name;
    }
    EXPECT_STREQ(result2str.c_str(), "DCEBA");
}

/*
 *   A
 *  ↙ ↘
 * D → B
 *  ↖ ↙ ↘
 *   C   E
 *
 * Expected: failure, circular dependency
 */
TEST(DependencyGraph, circular) {
    DependencyGraph graph;

    auto* nodeA = graph.addNode("A");
    auto* nodeB = graph.addNode("B");
    auto* nodeC = graph.addNode("C");
    auto* nodeD = graph.addNode("D");
    auto* nodeE = graph.addNode("E");

    nodeA->addEdge(nodeD);
    nodeA->addEdge(nodeB);
    nodeB->addEdge(nodeC);
    nodeB->addEdge(nodeE);
    nodeC->addEdge(nodeD);

    // Makes it circular
    nodeD->addEdge(nodeB);

    auto result1 = graph.resolveDependencyOrder();
    EXPECT_FALSE(result1);

    auto result2 = DependencyGraph::resolveDependencyOrder(nodeA);
    EXPECT_FALSE(result2);
}

/*
 *   A       F
 *  ↙ ↘     ↙ ↘
 * D   B   G   H
 *  ↖ ↙ ↘
 *   C   E
 *
 * Expected: D → C → E → B → A → G → H → F
 */
TEST(DependencyGraph, disconnected) {
    DependencyGraph graph;

    auto* nodeA = graph.addNode("A");
    auto* nodeB = graph.addNode("B");
    auto* nodeC = graph.addNode("C");
    auto* nodeD = graph.addNode("D");
    auto* nodeE = graph.addNode("E");
    auto* nodeF = graph.addNode("F");
    auto* nodeG = graph.addNode("G");
    auto* nodeH = graph.addNode("H");

    nodeA->addEdge(nodeD);
    nodeA->addEdge(nodeB);
    nodeB->addEdge(nodeC);
    nodeB->addEdge(nodeE);
    nodeC->addEdge(nodeD);

    nodeF->addEdge(nodeG);
    nodeF->addEdge(nodeH);

    // All nodes
    auto result1 = graph.resolveDependencyOrder();
    ASSERT_TRUE(result1);

    std::string result1str;
    for (const auto* item : *result1) {
        result1str += item->name;
    }
    EXPECT_STREQ(result1str.c_str(), "DCEBAGHF");

    // This one won't have the disconnected nodes (F + G + H)
    auto result2 = DependencyGraph::resolveDependencyOrder(nodeA);
    ASSERT_TRUE(result2);

    std::string result2str;
    for (const auto* item : *result2) {
        result2str += item->name;
    }
    EXPECT_STREQ(result2str.c_str(), "DCEBA");
}

/*
 *   A       F
 *  ↙ ↘     ↙ ↘
 * D   B → G   H
 *  ↖ ↙ ↘
 *   C   E
 *
 * Expected: D → C → E → G → B → A → H → F
 */
TEST(DependencyGraph, slightly_disconnected) {
    DependencyGraph graph;

    auto* nodeA = graph.addNode("A");
    auto* nodeB = graph.addNode("B");
    auto* nodeC = graph.addNode("C");
    auto* nodeD = graph.addNode("D");
    auto* nodeE = graph.addNode("E");
    auto* nodeF = graph.addNode("F");
    auto* nodeG = graph.addNode("G");
    auto* nodeH = graph.addNode("H");

    nodeA->addEdge(nodeD);
    nodeA->addEdge(nodeB);
    nodeB->addEdge(nodeC);
    nodeB->addEdge(nodeE);
    nodeC->addEdge(nodeD);

    nodeF->addEdge(nodeG);
    nodeF->addEdge(nodeH);

    nodeB->addEdge(nodeG);

    // All nodes
    auto result1 = graph.resolveDependencyOrder();
    ASSERT_TRUE(result1);

    std::string result1str;
    for (const auto* item : *result1) {
        result1str += item->name;
    }
    EXPECT_STREQ(result1str.c_str(), "DCEGBAHF");

    // This one won't have the disconnected nodes (F + H)
    auto result2 = DependencyGraph::resolveDependencyOrder(nodeA);
    ASSERT_TRUE(result2);

    std::string result2str;
    for (const auto* item : *result2) {
        result2str += item->name;
    }
    EXPECT_STREQ(result2str.c_str(), "DCEGBA");
}
