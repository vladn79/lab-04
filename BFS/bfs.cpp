#include "bfs.h"

std::vector<int> BFS(const Graph &graph, int start, std::vector<bool> &visited, std::vector<std::pair<int, int>> &edgesVisited) {
    std::vector<int> traversalOrder;
    std::queue<int> queue;
    visited[start] = true;
    queue.push(start);

    while (!queue.empty()) {
        int v = queue.front();
        queue.pop();
        traversalOrder.push_back(v);

        for (int u : graph.adjList[v]) {
            if (!visited[u]) {
                visited[u] = true;
                queue.push(u);
            }
            edgesVisited.push_back({v, u}); // Відвідуємо ребро
        }
    }

    return traversalOrder;
}
