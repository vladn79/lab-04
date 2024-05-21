#ifndef LAB04_BFS_H
#define LAB04_BFS_H
#pragma once
#include <vector>
struct Edge {
    int src, dest;
};

class Graph {
public:
    std::vector<std::vector<int>> adjList;
    Graph(const std::vector<Edge> &edges, int N);
};

std::vector<int> BFS(const Graph &graph, int start, std::vector<bool> &visited, std::vector<std::pair<int, int>> &edgesVisited);

#endif //LAB04_BFS_H
