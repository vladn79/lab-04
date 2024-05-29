#ifndef LAB04_JOHNSON_H
#define LAB04_JOHNSON_H

#include <vector>
#include <tuple>
#include <SFML/Graphics.hpp>
#include <queue>
#include <limits>

class Graph_johnson {
public:
    Graph_johnson(int vertices);
    void addEdge_johnson(int u, int v, int weight);
    void generateRandomGraph_johnson(int numEdges);
    bool johnsonStep_johnson();
    void draw_johnson(sf::RenderWindow& window);
    void reset_johnson();

private:
    void colorEdge_johnson(sf::RenderWindow& window, int u, int v, int weight, sf::Color color);
    void drawNodes_johnson(sf::RenderWindow& window);
    void drawEdgeWeight_johnson(sf::RenderWindow& window, int u, int v, int weight);
    void printEdgeInfo_johnson(int u, int v, int weight);
    bool bellmanFord_johnson(int src);
    bool dijkstra_johnson(int src);

    int vertices;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<std::vector<std::pair<int, int>>> adjList;
    std::vector<float> nodePositionsX;
    std::vector<float> nodePositionsY;
    std::vector<int> distances;
    std::vector<int> potentials;
    std::vector<bool> visited;
    std::vector<int> previous;

    using Edge = std::tuple<int, int, int>;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<>> pq;

    sf::Font font;
    bool fontLoaded;

    bool initialized;
    int step;
};


#endif //LAB04_JOHNSON_H
