#ifndef LAB04_KRUSKAL_H
#define LAB04_KRUSKAL_H

#include <vector>
#include <tuple>
#include <SFML/Graphics.hpp>


class Graph_krus {
public:
    Graph_krus(int vertices);
    void addEdge(int u, int v, int weight);
    void generateRandomGraph(int numEdges);
    bool kruskalStep();
    void draw(sf::RenderWindow& window);
    void reset(); // Скидає стан для нової візуалізації

private:
    int find(int i);
    void unionSets(int u, int v);
    void colorEdge(sf::RenderWindow& window, int u, int v, int weight, sf::Color color);
    void drawNodes(sf::RenderWindow& window);
    void printEdgeInfo(int u, int v, int weight);

    int vertices;
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<int> parent;
    std::vector<int> rank;
    std::vector<std::tuple<int, int, int>> mstEdges;
    std::vector<std::tuple<int, int, int>> allEdges;

    size_t currentEdge;

    sf::Font font;
    bool fontLoaded;
    std::vector<std::pair<float, float>> nodePositions;
};


#endif //LAB04_KRUSKAL_H
