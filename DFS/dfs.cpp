#include "dfs.h"
#include <stack>

std::vector<int> DFS(const Graph &graph, int start, std::vector<bool> &visited, std::vector<std::pair<int, int>> &edgesVisited) {
    std::vector<int> traversalOrder;
    std::stack<int> stack;
    stack.push(start);

    while (!stack.empty()) {
        int v = stack.top();
        stack.pop();

        if (!visited[v]) {
            visited[v] = true;
            traversalOrder.push_back(v);

            for (int u : graph.adjList[v]) {
                if (!visited[u]) {
                    stack.push(u);
                    edgesVisited.push_back({v, u});
                }
            }
        }
    }
    return traversalOrder;
}