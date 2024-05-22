#include "graph.h"

Graph::Graph(const std::vector<Edge> &edges, int N) {
    adjList.resize(N);
    for (const auto &edge : edges) {
        adjList[edge.src].push_back(edge.dest);
        adjList[edge.dest].push_back(edge.src);
    }
}
