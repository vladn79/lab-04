#ifndef LAB04_DFS_H
#define LAB04_DFS_H
#include "C:\projects\c++\labs\lab04\data_structures\graph.h"

std::vector<int> DFS(const Graph &graph, int start, std::vector<bool> &visited, std::vector<std::pair<int, int>> &edgesVisited);
#endif //LAB04_DFS_H
