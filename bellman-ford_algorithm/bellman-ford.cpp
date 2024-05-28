#include "bellman-ford.h"
#include <iostream>
#include <random>
#include <numeric>
#include <string>
#include <thread>
#include <chrono>
#include <limits>

Graph_bellman::Graph_bellman(int vertices) : vertices(vertices), distances(vertices, std::numeric_limits<float>::max()), previous(vertices, -1), currentStep(0), initialized(false), fontLoaded(false) {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Failed to load font 'arial.ttf'" << std::endl;
    } else {
        fontLoaded = true;
    }
}

void Graph_bellman::addEdge_bellman(int u, int v, int weight) {
    edges.push_back({weight, u, v});
}

void Graph_bellman::generateRandomGraph_bellman(int numEdges) {
    edges.clear();
    nodePositionsX.clear();
    nodePositionsY.clear();
    distances.assign(vertices, std::numeric_limits<float>::max());
    previous.assign(vertices, -1);
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
        nodePositionsX.push_back(x);
        nodePositionsY.push_back(y);
    }

    for (int i = 0; i < numEdges; ++i) {
        int u = vertexDist(gen);
        int v = vertexDist(gen);
        int weight = weightDist(gen);

        if (u != v) {
            addEdge_bellman(u, v, weight);
        }
    }

    startNode = 0;
    float minDistanceStart = nodePositionsX[0] * nodePositionsX[0] + nodePositionsY[0] * nodePositionsY[0];
    for (int i = 1; i < vertices; ++i) {
        float distanceStart = nodePositionsX[i] * nodePositionsX[i] + nodePositionsY[i] * nodePositionsY[i];
        if (distanceStart < minDistanceStart) {
            minDistanceStart = distanceStart;
            startNode = i;
        }
    }
}

bool Graph_bellman::bellmanStep_bellman() {
    if (!initialized) {
        initialized = true;
        distances[startNode] = 0;
    }

    if (currentStep > vertices) return false;

    bool updated = false;
    for (const auto& [weight, u, v] : edges) {
        if (distances[u] != std::numeric_limits<float>::max() && distances[u] + weight < distances[v]) {
            distances[v] = distances[u] + weight;
            previous[v] = u;
            updated = true;
        }
    }

    currentStep++;
    return updated || currentStep <= vertices;
}

void Graph_bellman::reset_bellman() {
    currentStep = 0;
    distances.assign(vertices, std::numeric_limits<float>::max());
    previous.assign(vertices, -1);
    initialized = false;
}

void Graph_bellman::draw_bellman(sf::RenderWindow& window) {
    for (const auto& [weight, u, v] : edges) {
        colorEdge_bellman(window, u, v, weight, sf::Color::Red);
    }

    for (int i = 0; i < vertices; ++i) {
        if (previous[i] != -1) {
            colorEdge_bellman(window, previous[i], i, distances[i] - distances[previous[i]], sf::Color::Green);
        }
    }

    drawNodes_bellman(window);
}

void Graph_bellman::colorEdge_bellman(sf::RenderWindow& window, int u, int v, int weight, sf::Color color) {
    auto x1 = nodePositionsX[u];
    auto y1 = nodePositionsY[u];
    auto x2 = nodePositionsX[v];
    auto y2 = nodePositionsY[v];

    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), color),
            sf::Vertex(sf::Vector2f(x2, y2), color)
    };
    window.draw(line, 2, sf::Lines);

    if (vertices <= 7) {
        drawEdgeWeight_bellman(window, u, v, weight);
    }
}

void Graph_bellman::drawNodes_bellman(sf::RenderWindow& window) {
    for (int i = 0; i < vertices; ++i) {
        sf::CircleShape node(7);
        node.setFillColor(sf::Color::White);
        auto x = nodePositionsX[i];
        auto y = nodePositionsY[i];
        node.setPosition(x - node.getRadius(), y - node.getRadius());
        window.draw(node);
    }
}

void Graph_bellman::drawEdgeWeight_bellman(sf::RenderWindow& window, int u, int v, int weight) {
    if (fontLoaded) {
        auto x1 = nodePositionsX[u];
        auto y1 = nodePositionsY[u];
        auto x2 = nodePositionsX[v];
        auto y2 = nodePositionsY[v];

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

void Graph_bellman::printEdgeInfo_bellman(int u, int v, int weight) {
    auto x1 = nodePositionsX[u];
    auto y1 = nodePositionsY[u];
    auto x2 = nodePositionsX[v];
    auto y2 = nodePositionsY[v];
    std::cout << "Edge from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ") with weight " << weight << std::endl;
}
