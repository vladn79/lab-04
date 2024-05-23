#include <SFML/Graphics.hpp>
#include "Kruskal's_algorithm/kruskal.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "BFS/bfs.h"
#include "DFS/dfs.h"
#include <random>
#include <cmath>
#include <vector>


std::vector<sf::Vector2f> generateRandomPoints(int N, float width, float height) {
    std::vector<sf::Vector2f> points(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(50, width - 50);
    std::uniform_real_distribution<> disY(50, height - 50);

    for (int i = 0; i < N; ++i) {
        points[i] = sf::Vector2f(disX(gen), disY(gen));
    }
    return points;
}


std::vector<Edge> generateRandomEdges(int N, int edgeCount) {
    std::vector<Edge> edges;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);

    for (int i = 1; i < N; ++i) {
        int src = dis(gen) % i;
        int dest = i;
        edges.push_back({src, dest});
    }

    for (int i = N; i < edgeCount; ++i) {
        int src = dis(gen);
        int dest = dis(gen);
        if (src != dest) {
            edges.push_back({src, dest});
        }
    }

    return edges;
}

void initializeNodes(std::vector<sf::CircleShape>& nodes, const std::vector<sf::Vector2f>& positions) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        nodes[i].setRadius(7);
        nodes[i].setFillColor(sf::Color::White);
        nodes[i].setPosition(positions[i]);
        nodes[i].setOrigin(nodes[i].getRadius(), nodes[i].getRadius());
    }
}

void visualizeGraph(const std::string& windowTitle, const Graph& graph, const std::vector<Edge>& edges, bool isBFS) {
    int N = graph.adjList.size();
    sf::RenderWindow window(sf::VideoMode(1500, 900), windowTitle);
    std::vector<sf::CircleShape> nodes(N);
    auto positions = generateRandomPoints(N, 1500, 900);
    initializeNodes(nodes, positions);

    std::vector<std::pair<int, int>> edgesVisited;
    std::vector<bool> visited(N, false);
    std::vector<int> traversalOrder;
    bool startSearch = false;
    size_t edgeIndex = 0;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!startSearch) {
                    if (isBFS) {
                        for (int i = 0; i < N; ++i) {
                            if (!visited[i]) {
                                std::vector<int> subTraversalOrder = BFS(graph, i, visited, edgesVisited);
                                traversalOrder.insert(traversalOrder.end(), subTraversalOrder.begin(), subTraversalOrder.end());
                            }
                        }
                    } else {
                        for (int i = 0; i < N; ++i) {
                            if (!visited[i]) {
                                std::vector<int> subTraversalOrder = DFS(graph, i, visited, edgesVisited);
                                traversalOrder.insert(traversalOrder.end(), subTraversalOrder.begin(), subTraversalOrder.end());
                            }
                        }
                    }
                    startSearch = true;
                } else if (edges.size() <= 25 && edgeIndex < edgesVisited.size()) {
                    edgeIndex++;
                }
            }
        }

        if (edges.size() > 25 && clock.getElapsedTime().asMilliseconds() > 2) {
            if (edgeIndex < edgesVisited.size()) {
                edgeIndex++;
            }
            clock.restart();
        }

        window.clear(sf::Color::Black);
        for (const auto &edge : edges) {
            sf::Vertex line[] = {
                    sf::Vertex(nodes[edge.src].getPosition(), sf::Color::White),
                    sf::Vertex(nodes[edge.dest].getPosition(), sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }

        for (size_t i = 0; i < edgeIndex && i < edgesVisited.size(); ++i) {
            sf::Vertex line[] = {
                    sf::Vertex(nodes[edgesVisited[i].first].getPosition(), sf::Color::Blue),
                    sf::Vertex(nodes[edgesVisited[i].second].getPosition(), sf::Color::Blue)
            };
            window.draw(line, 2, sf::Lines);
        }

        for (int i = 0; i < N; ++i) {
            window.draw(nodes[i]);
        }

        window.display();
    }
}



int main() {

    int N = 1000;
    int edgeCount = 1700;
    auto edges = generateRandomEdges(N, edgeCount);
    Graph graph1(edges, N);

    visualizeGraph("BFS Visualization", graph1, edges, true);

    visualizeGraph("DFS Visualization", graph1, edges, false);





    int vertices = 500;
    Graph_krus graph(vertices);

    graph.generateRandomGraph(900);

    sf::RenderWindow window(sf::VideoMode(750, 750), "Kruskal's Algorithm Visualization");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                graph.reset();
                graph.generateRandomGraph(900);
            }

            if (vertices <= 7 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!graph.kruskalStep()) {
                    std::cout << "MST Complete" << std::endl;
                }
            }
        }
        if (vertices > 7) {
            if (!graph.kruskalStep()) {
                std::cout << "MST Complete" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        window.clear();
        graph.draw(window);
        window.display();
    }

    return 0;
}