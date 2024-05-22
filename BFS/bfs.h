#ifndef LAB04_BFS_H
#define LAB04_BFS_H
#pragma once
#include "C:\projects\c++\labs\lab04\data_structures\graph.h"
#include <vector>
#include <queue>


std::vector<int> BFS(const Graph &graph, int start, std::vector<bool> &visited, std::vector<std::pair<int, int>> &edgesVisited);

#endif //LAB04_BFS_H
