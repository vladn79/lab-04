#ifndef LAB04_ASTAR_H
#define LAB04_ASTAR_H
#include <vector>
#include <tuple>
#include <SFML/Graphics.hpp>
#include <queue>
#include <limits>
#include <cmath>

class Graph_astar {
public:
    Graph_astar(int vertices);
    void addEdge_astar(int u, int v, int weight);
    void generateRandomGraph_astar(int numEdges);
    bool astarStep_astar();
    void draw_astar(sf::RenderWindow& window);
    void reset_astar();

private:
    void colorEdge_astar(sf::RenderWindow& window, int u, int v, int weight, sf::Color color);
    void drawNodes_astar(sf::RenderWindow& window);
    void drawEdgeWeight_astar(sf::RenderWindow& window, int u, int v, int weight);
    void printEdgeInfo_astar(int u, int v, int weight);
    float heuristic(int u, int v);

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
    int endNode;
};



#endif //LAB04_ASTAR_H
