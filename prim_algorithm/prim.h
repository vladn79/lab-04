#ifndef LAB04_PRIM_H
#define LAB04_PRIM_H

#include <vector>
#include <tuple>
#include <SFML/Graphics.hpp>
#include <queue>
class Graph_prima {
public:
    Graph_prima(int vertices);
    void addEdge_prima(int u, int v, int weight);
    void generateRandomGraph_prima(int numEdges);
    bool primaStep_prima();
    void draw_prima(sf::RenderWindow& window);
    void reset_prima();

private:
    void colorEdge_prima(sf::RenderWindow& window, int u, int v, int weight, sf::Color color);
    void drawNodes_prima(sf::RenderWindow& window);
    void drawEdgeWeight_prima(sf::RenderWindow& window, int u, int v, int weight);
    void printEdgeInfo_prima(int u, int v, int weight);

    int vertices;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<std::vector<std::pair<int, int>>> adjList;
    std::vector<bool> inMST;
    std::vector<std::tuple<int, int, int>> mstEdges;

    using Edge = std::tuple<int, int, int>;
    std::priority_queue<Edge, std::vector<Edge>, std::greater<>> pq;

    sf::Font font;
    bool fontLoaded;
    std::vector<std::pair<float, float>> nodePositions;

    bool initialized;
    int startNode;
};


#endif //LAB04_PRIM_H
