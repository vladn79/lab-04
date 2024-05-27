#include "astar.h"
#include <iostream>
#include <random>
#include <numeric>
#include <string>
#include <thread>
#include <chrono>
#include <limits>

Graph_astar::Graph_astar(int vertices) : vertices(vertices), adjList(vertices), distances(vertices, std::numeric_limits<float>::max()), visited(vertices, false), previous(vertices, -1), initialized(false), fontLoaded(false) {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Failed to load font 'arial.ttf'" << std::endl;
    } else {
        fontLoaded = true;
    }
}

void Graph_astar::addEdge_astar(int u, int v, int weight) {
    edges.push_back({weight, u, v});
    adjList[u].push_back({v, weight});
    adjList[v].push_back({u, weight});
}

void Graph_astar::generateRandomGraph_astar(int numEdges) {
    edges.clear();
    nodePositionsX.clear();
    nodePositionsY.clear();
    adjList.assign(vertices, {});
    distances.assign(vertices, std::numeric_limits<float>::max());
    visited.assign(vertices, false);
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
            addEdge_astar(u, v, weight);
        }
    }

    startNode = 0;
    endNode = 0;
    float minDistanceStart = nodePositionsX[0] * nodePositionsX[0] + nodePositionsY[0] * nodePositionsY[0];
    float minDistanceEnd = (1500 - nodePositionsX[0]) * (1500 - nodePositionsX[0]) + (900 - nodePositionsY[0]) * (900 - nodePositionsY[0]);
    for (int i = 1; i < vertices; ++i) {
        float distanceStart = nodePositionsX[i] * nodePositionsX[i] + nodePositionsY[i] * nodePositionsY[i];
        float distanceEnd = (1500 - nodePositionsX[i]) * (1500 - nodePositionsX[i]) + (900 - nodePositionsY[i]) * (900 - nodePositionsY[i]);
        if (distanceStart < minDistanceStart) {
            minDistanceStart = distanceStart;
            startNode = i;
        }
        if (distanceEnd < minDistanceEnd) {
            minDistanceEnd = distanceEnd;
            endNode = i;
        }
    }
}

bool Graph_astar::astarStep_astar() {
    if (!initialized) {
        initialized = true;
        distances[startNode] = 0;
        pq.push({0, startNode, startNode});
    }

    if (pq.empty()) return false;

    auto [dist, u, v] = pq.top();
    pq.pop();

    if (visited[v]) return true;

    visited[v] = true;
    printEdgeInfo_astar(u, v, dist);

    if (v == endNode) return false;

    for (const auto& [next, weight] : adjList[v]) {
        if (!visited[next]) {
            float tentative_g = distances[v] + weight;
            if (tentative_g < distances[next]) {
                distances[next] = tentative_g;
                previous[next] = v;
                float f = tentative_g + heuristic(next, endNode);
                pq.push({f, v, next});
            }
        }
    }

    return true;
}

float Graph_astar::heuristic(int u, int v) {
    return std::sqrt((nodePositionsX[u] - nodePositionsX[v]) * (nodePositionsX[u] - nodePositionsX[v]) + (nodePositionsY[u] - nodePositionsY[v]) * (nodePositionsY[u] - nodePositionsY[v]));
}

void Graph_astar::reset_astar() {
    initialized = false;
    distances.assign(vertices, std::numeric_limits<float>::max());
    visited.assign(vertices, false);
    previous.assign(vertices, -1);
    pq = std::priority_queue<Edge, std::vector<Edge>, std::greater<>>();
}

void Graph_astar::draw_astar(sf::RenderWindow& window) {
    for (const auto& [weight, u, v] : edges) {
        colorEdge_astar(window, u, v, weight, sf::Color::Red);
    }

    for (int i = 0; i < vertices; ++i) {
        if (previous[i] != -1) {
            colorEdge_astar(window, previous[i], i, distances[i] - distances[previous[i]], sf::Color::Green);
        }
    }

    drawNodes_astar(window);
}

void Graph_astar::colorEdge_astar(sf::RenderWindow& window, int u, int v, int weight, sf::Color color) {
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
        drawEdgeWeight_astar(window, u, v, weight);
    }
}

void Graph_astar::drawNodes_astar(sf::RenderWindow& window) {
    for (int i = 0; i < vertices; ++i) {
        sf::CircleShape node(7);
        node.setFillColor(sf::Color::White);
        auto x = nodePositionsX[i];
        auto y = nodePositionsY[i];
        node.setPosition(x - node.getRadius(), y - node.getRadius());
        window.draw(node);
    }
}

void Graph_astar::drawEdgeWeight_astar(sf::RenderWindow& window, int u, int v, int weight) {
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

void Graph_astar::printEdgeInfo_astar(int u, int v, int weight) {
    auto x1 = nodePositionsX[u];
    auto y1 = nodePositionsY[u];
    auto x2 = nodePositionsX[v];
    auto y2 = nodePositionsY[v];
    std::cout << "Edge from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ") with weight " << weight << std::endl;
}

