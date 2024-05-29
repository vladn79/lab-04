#include "ford_fulkerson.h"
#include <iostream>
#include <random>
#include <numeric>
#include <string>
#include <thread>
#include <chrono>
#include <limits>

Graph_fordFulkerson::Graph_fordFulkerson(int vertices)
        : vertices(vertices), adjList(vertices), capacity(vertices, std::vector<int>(vertices, 0)), parent(vertices), visited(vertices), fontLoaded(false), maxFlow(0), step(0), source(0), sink(vertices - 1) {
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Failed to load font 'arial.ttf'" << std::endl;
    } else {
        fontLoaded = true;
    }
}

void Graph_fordFulkerson::addEdge_fordFulkerson(int u, int v, int cap) {
    edges.push_back({cap, u, v});
    capacity[u][v] = cap;
    adjList[u].push_back(v);
    adjList[v].push_back(u);  // Add reverse edge for residual graph
}

void Graph_fordFulkerson::generateRandomGraph_fordFulkerson(int numEdges) {
    edges.clear();
    nodePositionsX.clear();
    nodePositionsY.clear();
    adjList.assign(vertices, {});
    capacity.assign(vertices, std::vector<int>(vertices, 0));
    parent.assign(vertices, -1);
    visited.assign(vertices, false);
    maxFlow = 0;
    step = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> vertexDist(0, vertices - 1);
    std::uniform_int_distribution<> capacityDist(1, 20);
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
        int cap = capacityDist(gen);

        if (u != v) {
            addEdge_fordFulkerson(u, v, cap);
        }
    }
}

bool Graph_fordFulkerson::bfs_fordFulkerson(int source, int sink, std::vector<int>& parent) {
    std::fill(visited.begin(), visited.end(), false);
    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adjList[u]) {
            if (!visited[v] && capacity[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == sink) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Graph_fordFulkerson::fordFulkersonStep_fordFulkerson() {
    if (step == 0) {
        step = 1;
    }

    if (step == 1) {
        if (!bfs_fordFulkerson(source, sink, parent)) {
            return false;
        }

        int pathFlow = std::numeric_limits<int>::max();
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, capacity[u][v]);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            capacity[u][v] -= pathFlow;
            capacity[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
        printEdgeInfo_fordFulkerson(source, sink, maxFlow);
    }

    return true;
}

void Graph_fordFulkerson::reset_fordFulkerson() {
    maxFlow = 0;
    step = 0;
    parent.assign(vertices, -1);
    visited.assign(vertices, false);
    capacity.assign(vertices, std::vector<int>(vertices, 0));
    adjList.assign(vertices, {});
}

void Graph_fordFulkerson::draw_fordFulkerson(sf::RenderWindow& window) {
    for (const auto& [capacity, u, v] : edges) {
        colorEdge_fordFulkerson(window, u, v, capacity, sf::Color::Red);
    }

    for (int i = 0; i < vertices; ++i) {
        if (parent[i] != -1) {
            colorEdge_fordFulkerson(window, parent[i], i, capacity[parent[i]][i], sf::Color::Green);
        }
    }

    drawNodes_fordFulkerson(window);
}

void Graph_fordFulkerson::colorEdge_fordFulkerson(sf::RenderWindow& window, int u, int v, int capacity, sf::Color color) {
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
        drawEdgeCapacity_fordFulkerson(window, u, v, capacity);
    }
}

void Graph_fordFulkerson::drawNodes_fordFulkerson(sf::RenderWindow& window) {
    for (int i = 0; i < vertices; ++i) {
        sf::CircleShape node(3);
        node.setFillColor(sf::Color::White);
        auto x = nodePositionsX[i];
        auto y = nodePositionsY[i];
        node.setPosition(x - node.getRadius(), y - node.getRadius());
        window.draw(node);
    }
}

void Graph_fordFulkerson::drawEdgeCapacity_fordFulkerson(sf::RenderWindow& window, int u, int v, int capacity) {
    if (fontLoaded) {
        auto x1 = nodePositionsX[u];
        auto y1 = nodePositionsY[u];
        auto x2 = nodePositionsX[v];
        auto y2 = nodePositionsY[v];

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(capacity));
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

void Graph_fordFulkerson::printEdgeInfo_fordFulkerson(int u, int v, int capacity) {
    auto x1 = nodePositionsX[u];
    auto y1 = nodePositionsY[u];
    auto x2 = nodePositionsX[v];
    auto y2 = nodePositionsY[v];
    std::cout << "Edge from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ") with capacity " << capacity << std::endl;
}
