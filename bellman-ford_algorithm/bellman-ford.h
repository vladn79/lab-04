#ifndef LAB04_BELLMAN_FORD_H
#define LAB04_BELLMAN_FORD_H
#include <vector>
#include <tuple>
#include <SFML/Graphics.hpp>

class Graph_bellman {
public:
    Graph_bellman(int vertices);
    void addEdge_bellman(int u, int v, int weight);
    void generateRandomGraph_bellman(int numEdges);
    bool bellmanStep_bellman();
    void draw_bellman(sf::RenderWindow& window);
    void reset_bellman();

private:
    void colorEdge_bellman(sf::RenderWindow& window, int u, int v, int weight, sf::Color color);
    void drawNodes_bellman(sf::RenderWindow& window);
    void drawEdgeWeight_bellman(sf::RenderWindow& window, int u, int v, int weight);
    void printEdgeInfo_bellman(int u, int v, int weight);

    int vertices;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<float> nodePositionsX;
    std::vector<float> nodePositionsY;
    std::vector<float> distances;
    std::vector<int> previous;

    int currentStep;
    int startNode;

    sf::Font font;
    bool fontLoaded;

    bool initialized;
};



#endif //LAB04_BELLMAN_FORD_H
