#include "johnson.h"
#include <iostream>
#include <random>
#include <numeric>
#include <string>
#include <thread>
#include <chrono>
#include <limits>

Graph_johnson::Graph_johnson(int vertices)
        : vertices(vertices), adjList(vertices + 1), distances(vertices + 1, std::numeric_limits<int>::max()), potentials(vertices + 1, 0), visited(vertices + 1, false), previous(vertices + 1, -1), initialized(false), fontLoaded(false), step(0) {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Failed to load font 'arial.ttf'" << std::endl;
    } else {
        fontLoaded = true;
    }
}

void Graph_johnson::addEdge_johnson(int u, int v, int weight) {
    edges.push_back({weight, u, v});
    adjList[u].push_back({v, weight});
}

void Graph_johnson::generateRandomGraph_johnson(int numEdges) {
    edges.clear();
    nodePositionsX.clear();
    nodePositionsY.clear();
    adjList.assign(vertices + 1, {});
    distances.assign(vertices + 1, std::numeric_limits<int>::max());
    potentials.assign(vertices + 1, 0);
    visited.assign(vertices + 1, false);
    previous.assign(vertices + 1, -1);
    initialized = false;
    step = 0;

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
            addEdge_johnson(u, v, weight);
        }
    }
}

bool Graph_johnson::bellmanFord_johnson(int src) {
    distances[src] = 0;
    for (int i = 0; i < vertices; ++i) {
        for (const auto& [weight, u, v] : edges) {
            if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
            }
        }
    }

    for (const auto& [weight, u, v] : edges) {
        if (distances[u] != std::numeric_limits<int>::max() && distances[u] + weight < distances[v]) {
            return false;
        }
    }

    for (int i = 0; i < vertices; ++i) {
        potentials[i] = distances[i];
    }

    return true;
}

bool Graph_johnson::dijkstra_johnson(int src) {
    distances.assign(vertices + 1, std::numeric_limits<int>::max());
    visited.assign(vertices + 1, false);
    pq = std::priority_queue<Edge, std::vector<Edge>, std::greater<>>();

    distances[src] = 0;
    pq.push({0, src, src});

    while (!pq.empty()) {
        auto [dist, u, v] = pq.top();
        pq.pop();

        if (visited[v]) continue;
        visited[v] = true;

        for (const auto& [next, weight] : adjList[v]) {
            int newDist = dist + weight + potentials[v] - potentials[next];
            if (newDist < distances[next]) {
                distances[next] = newDist;
                previous[next] = v;
                pq.push({newDist, v, next});
            }
        }
    }

    return true;
}

bool Graph_johnson::johnsonStep_johnson() {
    if (!initialized) {
        initialized = true;
        if (!bellmanFord_johnson(vertices)) {
            std::cerr << "Graph contains a negative-weight cycle" << std::endl;
            return false;
        }
        step = 0;
    }

    if (step >= vertices) return false;

    dijkstra_johnson(step);

    step++;
    return true;
}

void Graph_johnson::reset_johnson() {
    initialized = false;
    distances.assign(vertices + 1, std::numeric_limits<int>::max());
    visited.assign(vertices + 1, false);
    previous.assign(vertices + 1, -1);
    pq = std::priority_queue<Edge, std::vector<Edge>, std::greater<>>();
    step = 0;
}

void Graph_johnson::draw_johnson(sf::RenderWindow& window) {
    for (const auto& [weight, u, v] : edges) {
        colorEdge_johnson(window, u, v, weight, sf::Color::Red);
    }

    for (int i = 0; i < vertices; ++i) {
        if (previous[i] != -1) {
            colorEdge_johnson(window, previous[i], i, distances[i] - distances[previous[i]], sf::Color::Green);
        }
    }

    drawNodes_johnson(window);
}

void Graph_johnson::colorEdge_johnson(sf::RenderWindow& window, int u, int v, int weight, sf::Color color) {
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
        drawEdgeWeight_johnson(window, u, v, weight);
    }
}

void Graph_johnson::drawNodes_johnson(sf::RenderWindow& window) {
    for (int i = 0; i < vertices; ++i) {
        sf::CircleShape node(3);
        node.setFillColor(sf::Color::White);
        auto x = nodePositionsX[i];
        auto y = nodePositionsY[i];
        node.setPosition(x - node.getRadius(), y - node.getRadius());
        window.draw(node);
    }
}

void Graph_johnson::drawEdgeWeight_johnson(sf::RenderWindow& window, int u, int v, int weight) {
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

void Graph_johnson::printEdgeInfo_johnson(int u, int v, int weight) {
    auto x1 = nodePositionsX[u];
    auto y1 = nodePositionsY[u];
    auto x2 = nodePositionsX[v];
    auto y2 = nodePositionsY[v];
    std::cout << "Edge from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ") with weight " << weight << std::endl;
}
