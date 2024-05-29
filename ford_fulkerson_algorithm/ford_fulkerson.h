#ifndef LAB04_FORD_FULKERSON_H
#define LAB04_FORD_FULKERSON_H
#include <vector>
#include <tuple>
#include <SFML/Graphics.hpp>
#include <limits>
#include <queue>

class Graph_fordFulkerson {
public:
    Graph_fordFulkerson(int vertices);
    void addEdge_fordFulkerson(int u, int v, int capacity);
    void generateRandomGraph_fordFulkerson(int numEdges);
    bool fordFulkersonStep_fordFulkerson();
    void draw_fordFulkerson(sf::RenderWindow& window);
    void reset_fordFulkerson();

private:
    void colorEdge_fordFulkerson(sf::RenderWindow& window, int u, int v, int capacity, sf::Color color);
    void drawNodes_fordFulkerson(sf::RenderWindow& window);
    void drawEdgeCapacity_fordFulkerson(sf::RenderWindow& window, int u, int v, int capacity);
    void printEdgeInfo_fordFulkerson(int u, int v, int capacity);
    bool bfs_fordFulkerson(int source, int sink, std::vector<int>& parent);

    int vertices;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<std::vector<int>> capacity;
    std::vector<std::vector<int>> adjList;
    std::vector<float> nodePositionsX;
    std::vector<float> nodePositionsY;
    std::vector<int> parent;
    std::vector<bool> visited;

    sf::Font font;
    bool fontLoaded;

    int maxFlow;
    int step;
    int source;
    int sink;
};


#endif //LAB04_FORD_FULKERSON_H
