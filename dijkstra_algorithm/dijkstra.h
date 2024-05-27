#ifndef LAB04_DIJKSTRA_H
#define LAB04_DIJKSTRA_H

#include <vector>
#include <tuple>
#include <SFML/Graphics.hpp>
#include <queue>
#include <limits>

class Graph_dijkstra {
public:
    Graph_dijkstra(int vertices);
    void addEdge_dijkstra(int u, int v, int weight);
    void generateRandomGraph_dijkstra(int numEdges);
    bool dijkstraStep_dijkstra();
    void draw_dijkstra(sf::RenderWindow& window);
    void reset_dijkstra();

private:
    void colorEdge_dijkstra(sf::RenderWindow& window, int u, int v, int weight, sf::Color color);
    void drawNodes_dijkstra(sf::RenderWindow& window);
    void drawEdgeWeight_dijkstra(sf::RenderWindow& window, int u, int v, int weight);
    void printEdgeInfo_dijkstra(int u, int v, int weight);

    int vertices;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<std::vector<std::pair<int, int>>> adjList;
    std::vector<float> nodePositionsX;
    std::vector<float> nodePositionsY;
    std::vector<float> distances;
    std::vector<bool> visited;
    std::vector<int> previous;

    using Edge = std::tuple<int, int, int>;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<>> pq;

    sf::Font font;
    bool fontLoaded;

    bool initialized;
    int startNode;
};



#endif //LAB04_DIJKSTRA_H
