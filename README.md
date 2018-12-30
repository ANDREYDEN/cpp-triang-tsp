# C++ TSP library

This library includes C++ implementation of different algs for computing triangulations and [TSP](https://en.wikipedia.org/wiki/Travelling_salesman_problem) approximations.

There are in fact 3 libraries: geometry, triangulation computing, TSP approximations computing.

## Examples
### Triangulation
![triang](Gallery/triangulation(ALL).png)
### TSP tour (Cheapest edges approximation)
![tsp](Gallery/improved(2).png)


## Algorithms

1. [Bowyer-Watson algorithm](https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm) for computing [Delaunay Triangulation](https://en.wikipedia.org/wiki/Delaunay_triangulation).
2. Cheapest Link Algorithm
3. [Kruskal's Algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm)
4. [MST](https://en.wikipedia.org/wiki/Minimum_spanning_tree) + [DFS] (https://en.wikipedia.org/wiki/Depth-first_search) TSP [approximation](https://en.wikipedia.org/wiki/Travelling_salesman_problem#Heuristic_and_approximation_algorithms).
5. Cheapest Edges + DFS TSP approximation.

## References and inspirations

1. Bowyer-Watson algorithm and improvements: http://www.gdmc.nl/publications/2002/Bowyer_Watson_algorithm.pdf
2. MST + DFS TSP approximation:
3. Images from [this](https://www.kaggle.com/c/traveling-santa-2018-prime-paths) Kaggle competition.
