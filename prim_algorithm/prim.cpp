#include "prim.h"
#include <iostream>
#include <random>
#include <numeric>
#include <string>
#include <thread>
#include <chrono>

Graph_prima::Graph_prima(int vertices) : vertices(vertices), adjList(vertices), inMST(vertices, false), initialized(false), startNode(0), fontLoaded(false) {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Failed to load font 'arial.ttf'" << std::endl;
    } else {
        fontLoaded = true;
    }
}

void Graph_prima::addEdge_prima(int u, int v, int weight) {
    edges.push_back({weight, u, v});
    adjList[u].push_back({v, weight});
    adjList[v].push_back({u, weight});
}

void Graph_prima::generateRandomGraph_prima(int numEdges) {
    edges.clear();
    mstEdges.clear();
    nodePositions.clear();
    adjList.assign(vertices, {});
    inMST.assign(vertices, false);
    initialized = false;

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
            addEdge_prima(u, v, weight);
        }
    }
}

bool Graph_prima::primaStep_prima() {
    if (!initialized) {
        initialized = true;
        inMST[startNode] = true;
        for (const auto& [v, weight] : adjList[startNode]) {
            pq.push({weight, startNode, v});
        }
    }

    if (pq.empty()) return false;

    auto [weight, u, v] = pq.top();
    pq.pop();

    if (inMST[v]) return true;

    mstEdges.push_back({weight, u, v});
    inMST[v] = true;

    printEdgeInfo_prima(u, v, weight);

    for (const auto& [next, nextWeight] : adjList[v]) {
        if (!inMST[next]) {
            pq.push({nextWeight, v, next});
        }
    }

    return true;
}

void Graph_prima::reset_prima() {
    initialized = false;
    mstEdges.clear();
    pq = std::priority_queue<Edge, std::vector<Edge>, std::greater<>>();
    inMST.assign(vertices, false);
}

void Graph_prima::draw_prima(sf::RenderWindow& window) {
    for (const auto& [weight, u, v] : edges) {
        colorEdge_prima(window, u, v, weight, sf::Color::Red);
    }
    for (const auto& [weight, u, v] : mstEdges) {
        colorEdge_prima(window, u, v, weight, sf::Color::Green);
    }
    drawNodes_prima(window);
}

void Graph_prima::colorEdge_prima(sf::RenderWindow& window, int u, int v, int weight, sf::Color color) {
    auto [x1, y1] = nodePositions[u];
    auto [x2, y2] = nodePositions[v];

    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), color),
            sf::Vertex(sf::Vector2f(x2, y2), color)
    };
    window.draw(line, 2, sf::Lines);

    if (vertices <= 7) {
        drawEdgeWeight_prima(window, u, v, weight);
    }
}

void Graph_prima::drawNodes_prima(sf::RenderWindow& window) {
    for (int i = 0; i < vertices; ++i) {
        sf::CircleShape node(3);
        node.setFillColor(sf::Color::White);
        auto [x, y] = nodePositions[i];
        node.setPosition(x - node.getRadius(), y - node.getRadius());
        window.draw(node);
    }
}

void Graph_prima::drawEdgeWeight_prima(sf::RenderWindow& window, int u, int v, int weight) {
    if (fontLoaded) {
        auto [x1, y1] = nodePositions[u];
        auto [x2, y2] = nodePositions[v];

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

void Graph_prima::printEdgeInfo_prima(int u, int v, int weight) {
    auto [x1, y1] = nodePositions[u];
    auto [x2, y2] = nodePositions[v];
    std::cout << "Edge from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ") with weight " << weight << std::endl;
}