#include <SFML/Graphics.hpp>
#include "kruskals_algorithm/kruskal.h"
#include "prim_algorithm/prim.h"
#include "dijkstra_algorithm/dijkstra.h"
#include "astar_algorithm/astar.h"
#include "bellman-ford_algorithm/bellman-ford.h"
#include "johnson_algorithm/johnson.h"
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




    int vertices = 1000;
   Graph_krus graph(vertices);

    graph.generateRandomGraph(1700);

    sf::RenderWindow window(sf::VideoMode(1500, 900), "Kruskal's Algorithm Visualization");
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


    Graph_prima graph_prima(vertices);

    graph_prima.generateRandomGraph_prima(900);

    sf::RenderWindow window1(sf::VideoMode(1500, 900), "Prim's Algorithm Visualization");
    window1.setFramerateLimit(60);

    while (window1.isOpen()) {
        sf::Event event;
        while (window1.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window1.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                graph_prima.reset_prima();
                graph_prima.generateRandomGraph_prima(900);
            }

            if (vertices <= 7 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!graph_prima.primaStep_prima()) {
                    std::cout << "MST Complete" << std::endl;
                }
            }
        }
        if (vertices > 7) {
            if (!graph_prima.primaStep_prima()) {
                std::cout << "MST Complete" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        window1.clear();
        graph_prima.draw_prima(window1);
        window1.display();
    }


    Graph_dijkstra graph_dijkstra(vertices);

    graph_dijkstra.generateRandomGraph_dijkstra(900);

    sf::RenderWindow window2(sf::VideoMode(1500, 900), "Dijkstra's Algorithm Visualization");
    window2.setFramerateLimit(60);

    while (window2.isOpen()) {
        sf::Event event;
        while (window2.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window2.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                graph_dijkstra.reset_dijkstra();
                graph_dijkstra.generateRandomGraph_dijkstra(900);
            }

            if (vertices <= 7 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!graph_dijkstra.dijkstraStep_dijkstra()) {
                    std::cout << "Dijkstra Complete" << std::endl;
                }
            }
        }
        if (vertices > 7) {
            if (!graph_dijkstra.dijkstraStep_dijkstra()) {
                std::cout << "Dijkstra Complete" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        window2.clear();
        graph_dijkstra.draw_dijkstra(window2);
        window2.display();
    }


    Graph_astar graph_astar(vertices);
    graph_astar.generateRandomGraph_astar(900);

    sf::RenderWindow window3(sf::VideoMode(1500, 900), "A* Algorithm Visualization");
    window3.setFramerateLimit(60);

    while (window3.isOpen()) {
        sf::Event event;
        while (window3.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window3.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                graph_astar.reset_astar();
                graph_astar.generateRandomGraph_astar(900);
            }

            if (vertices <= 7 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!graph_astar.astarStep_astar()) {
                    std::cout << "A* Complete" << std::endl;
                }
            }
        }

        if (vertices > 7) {
            if (!graph_astar.astarStep_astar()) {
                std::cout << "A* Complete" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        window3.clear();
        graph_astar.draw_astar(window3);
        window3.display();
    }



   /*Graph_bellman graph_bel(vertices);
    graph_bel.generateRandomGraph_bellman(900);

    sf::RenderWindow window4(sf::VideoMode(1500, 900), "Bellman-Ford Algorithm Visualization");
    window4.setFramerateLimit(60);

    while (window4.isOpen()) {
        sf::Event event;
        while (window4.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                graph_bel.reset_bellman();
                graph_bel.generateRandomGraph_bellman(900);
            }

            if (vertices <= 7 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!graph_bel.bellmanStep_bellman()) {
                    std::cout << "Bellman-Ford Complete" << std::endl;
                }
            }
        }

        if (vertices > 7) {
            if (!graph_bel.bellmanStep_bellman()) {
                std::cout << "Bellman-Ford Complete" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        window4.clear();
        graph_bel.draw_bellman(window4);
        window4.display();
    }*/

    Graph_johnson graph_johnson(vertices);

    graph_johnson.generateRandomGraph_johnson(900);

    sf::RenderWindow window5(sf::VideoMode(1500, 900), "Johnson's Algorithm Visualization");
    window5.setFramerateLimit(60);

    while (window5.isOpen()) {
        sf::Event event;
        while (window5.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window5.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                graph_johnson.reset_johnson();
                graph_johnson.generateRandomGraph_johnson(900);
            }

            if (vertices <= 7 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!graph_johnson.johnsonStep_johnson()) {
                    std::cout << "Johnson Complete" << std::endl;
                }
            }
        }
        if (vertices > 7) {
            if (!graph_johnson.johnsonStep_johnson()) {
                std::cout << "Johnson Complete" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        window5.clear();
        graph_johnson.draw_johnson(window5);
        window5.display();
    }


    return 0;
}