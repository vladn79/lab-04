#include "kruskal.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <random>


Graph_krus::Graph_krus(int vertices)  : vertices(vertices), parent(vertices), rank(vertices, 0), currentEdge(0), fontLoaded(false) {
    std::iota(parent.begin(), parent.end(), 0);


    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Failed to load font 'arial.ttf'" << std::endl;
    } else {
        fontLoaded = true;
    }
}

void Graph_krus::addEdge(int u, int v, int weight) {
    edges.push_back({weight, u, v});
    allEdges.push_back({weight, u, v});
}

void Graph_krus::generateRandomGraph(int numEdges) {
    edges.clear();
    allEdges.clear();
    nodePositions.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> vertexDist(0, vertices - 1);
    std::uniform_int_distribution<> weightDist(1, 20);
    std::uniform_real_distribution<float> posDistX(50, 1450);
    std::uniform_real_distribution<float> posDistY(50, 850);

    for (int i = 0; i < vertices; ++i) {
        float x = posDistX(gen);
        float y = posDistY(gen);
        nodePositions.push_back({x, y});
    }

    for (int i = 0; i < numEdges; ++i) {
        int u = vertexDist(gen);
        int v = vertexDist(gen);
        int weight = weightDist(gen);

        if (u != v) {
            addEdge(u, v, weight);
        }
    }
}

int Graph_krus::find(int i) {
    if (i != parent[i]) {
        parent[i] = find(parent[i]);
    }
    return parent[i];
}

void Graph_krus::unionSets(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);
    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        } else if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        } else {
            parent[rootV] = rootU;
            ++rank[rootU];
        }
    }
}

bool Graph_krus::kruskalStep() {
    if (currentEdge >= edges.size()) return false;
    std::sort(edges.begin(), edges.end());
    auto [weight, u, v] = edges[currentEdge];
    if (find(u) != find(v)) {
        mstEdges.push_back({weight, u, v});
        unionSets(u, v);
        printEdgeInfo(u, v, weight);
    }
    currentEdge++;
    return true;
}

void Graph_krus::reset() {
    currentEdge = 0;
    mstEdges.clear();
    std::iota(parent.begin(), parent.end(), 0);
    std::fill(rank.begin(), rank.end(), 0);
}

void Graph_krus::draw(sf::RenderWindow& window) {

    for (const auto& [weight, u, v] : allEdges) {
        colorEdge(window, u, v, weight, sf::Color::Red);
    }
    for (const auto& [weight, u, v] : mstEdges) {
        colorEdge(window, u, v, weight, sf::Color::Green);
    }
    drawNodes(window);
}

void Graph_krus::colorEdge(sf::RenderWindow& window, int u, int v, int weight, sf::Color color) {
    auto [x1, y1] = nodePositions[u];
    auto [x2, y2] = nodePositions[v];

    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), color),
            sf::Vertex(sf::Vector2f(x2, y2), color)
    };
    window.draw(line, 2, sf::Lines);


    if (fontLoaded && edges.size() <= 10) {
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(weight));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);

        float textX = (x1 + x2) / 2;
        float textY = (y1 + y2) / 2;


        textX += 5;
        textY += 5;

        text.setPosition(textX, textY);

        window.draw(text);
    }
}

void Graph_krus::drawNodes(sf::RenderWindow& window) {
    for (int i = 0; i < vertices; ++i) {
        sf::CircleShape node(7);
        node.setFillColor(sf::Color::White);
        auto [x, y] = nodePositions[i];
        node.setPosition(x - node.getRadius(), y - node.getRadius());
        window.draw(node);
    }
}

void Graph_krus::printEdgeInfo(int u, int v, int weight) {
    auto [x1, y1] = nodePositions[u];
    auto [x2, y2] = nodePositions[v];
    std::cout << "Edge from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ") with weight " << weight << std::endl;
}