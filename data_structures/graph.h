#ifndef LAB04_GRAPH_H
#define LAB04_GRAPH_H
#include <vector>

struct Edge {
    int src, dest;
};

class Graph {
public:
    std::vector<std::vector<int>> adjList;
    Graph(const std::vector<Edge> &edges, int N);
};

#endif //LAB04_GRAPH_H
