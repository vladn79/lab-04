cmake_minimum_required(VERSION 3.27)
project(lab04)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab04
        DFS/dfs.h
        DFS/dfs.cpp
        BFS/bfs.h
        BFS/bfs.cpp
        kruskals_algorithm/kruskal.h
        kruskals_algorithm/kruskal.cpp
        main.cpp
        data_structures/graph.cpp
        prim_algorithm/prim.h
        prim_algorithm/prim.cpp
        dijkstra_algorithm/dijkstra.h
        dijkstra_algorithm/dijkstra.cpp
        astar_algorithm/astar.h
        astar_algorithm/astar.cpp
        bellman-ford_algorithm/bellman-ford.h
        bellman-ford_algorithm/bellman-ford.cpp
        johnson_algorithm/johnson.h
        johnson_algorithm/johnson.cpp
        ford_fulkerson_algorithm/ford_fulkerson.h
        ford_fulkerson_algorithm/ford_fulkerson.cpp

)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Users/38093/OneDrive/Документи/libs/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/Users/38093/OneDrive/Документи/libs/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/include)

target_link_libraries(lab04 sfml-system sfml-window sfml-graphics sfml-audio)
