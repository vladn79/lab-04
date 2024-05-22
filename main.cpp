#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "BFS/bfs.h"
#include "DFS/dfs.h"

void initializeNodes(std::vector<sf::CircleShape>& nodes, const std::vector<sf::Vector2f>& positions) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        nodes[i].setRadius(20);
        nodes[i].setFillColor(sf::Color::White);
        nodes[i].setPosition(positions[i]);
        nodes[i].setOrigin(nodes[i].getRadius(), nodes[i].getRadius());
    }
}
void visualizeGraph(const std::string& windowTitle, const Graph& graph, const std::vector<Edge>& edges, bool isBFS) {
    int N = graph.adjList.size();
    sf::RenderWindow window(sf::VideoMode(800, 800), windowTitle);
    std::vector<sf::CircleShape> nodes(N);
    std::vector<sf::Vector2f> positions = { {100, 100}, {200, 200}, {300, 100}, {400, 200}, {500, 100}, {600, 200}, {700, 100}, {500, 300}, {300, 300} };
    initializeNodes(nodes, positions);

    std::vector<std::pair<int, int>> edgesVisited;
    std::vector<bool> visited(N, false);
    std::vector<int> traversalOrder;
    bool startSearch = false;
    size_t edgeIndex = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (!startSearch) {
                        if (isBFS) {
                            traversalOrder = BFS(graph, 0, visited, edgesVisited);
                        } else {
                            traversalOrder = DFS(graph, 0, visited, edgesVisited);
                        }
                        startSearch = true;
                    } else if (edgeIndex < edgesVisited.size()) {
                        edgeIndex++;
                    }
                }
            }
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
    std::vector<Edge> edges = { {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8} };
    int N = 9;
    Graph graph(edges, N);

    visualizeGraph("BFS Visualization", graph, edges, true);

    visualizeGraph("DFS Visualization", graph, edges, false);

    return 0;
}