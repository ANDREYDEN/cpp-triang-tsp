# C++ TSP library

This library includes C++ implementation of different algs for computing triangulations and [TSP](https://en.wikipedia.org/wiki/Travelling_salesman_problem) approximations.

There are in fact 3 libraries: geometry, triangulation computing, TSP approximations computing.

## Examples
### Triangulation
![triang](Gallery/triangulation(ALL).png)
### TSP tour (Cheapest edges approximation)
![tsp](Gallery/improved(2).png)
### MST (based on the trianguation)
![mst](Gallery/MST(2.2).png)

## Algorithms

1. [Bowyer-Watson algorithm](https://en.wikipedia.org/wiki/Bowyer%E2%80%93Watson_algorithm) for computing [Delaunay Triangulation](https://en.wikipedia.org/wiki/Delaunay_triangulation).
2. [Cheapest Link Algorithm](http://www.jlmartin.faculty.ku.edu/courses/math105-F14/chapter6-part5.pdf)
3. [Kruskal's Algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm)
4. [2-approximation](http://www.dis.uniroma1.it/~alberto/didattica/tcs-2011/alberto/TCS-approx-4.pdf) algorithm.
5. Cheapest Edges + DFS approximation.
6. Delaunay Triangulation + Iterative Point Addition

## Performance

| Name                            | Result        | Time     | Memory   | Optimality  |
| -                               | :-:           | -        | -        | :-:         |
| B-W                             | Triangulation | O(VlogV) | O(VlogV) | Optimal     |
| CLA (pointset)                  | TSP tour      | O(VlogV) | O(V^2)   | 1.5 Optimal |
| Kruskal (pointset)              | MST           | O(VlogV) | O(V^2)   | Optimal     |
| 2-approximation (pointset)      | TSP tour      | O(VlogV) | O(V^2)   | 2 Optimal   |
| 2-approximation (triangulation) | TSP tour      | O(VlogV) | O(VlogV) | ?           |
| CE + DFS                        | TSP tour      | O(VlogV) | O(VlogV) | ?           |
| DT + IPA                        | TSP tour      | O(VlogV) | O(VlogV) | ?           |


## Usage

The library works with text files.

input.txt (coordinates of points)
```
316.83673906150904 2202.34070733524
4377.40597216624 336.602082171235
3454.15819771172 2820.0530112481106
4688.099297634771 2935.89805580997
1010.6969517482901 3236.75098902635
...
```

### Triangulation
main.cpp
```
#include "triangulation.h"
#include "geometry.h"

using namespace std;

int main() {
    BowyerWatson bw;
    // reading points from a file with coordinates
    bw.fromCoords("input.txt");
    // computing the triangulation
    bw.compute();
    // writing to a file
    bw.toEdges("triang.txt");
}
```

triang.txt (edges of triangulation; each number represents a point according to the input sequence)
```
197338 88708
88708 20109
88708 12785
88708 123994
88708 123616
...
```

### TSP
main.cpp
```
#include "geometry.h"
#include "tsp.h"
#include <bits/stdc++.h>

using namespace std;

int main()
{
    // read coordinates from file
    vector<Point> coords = toPoints("input.txt");
    TSP tsp(toEdges("triang.txt", coords, 3));
    // creating an edge subset for faster solution finding
    tsp.toSmallestEdges();
    // computing the tour
    tsp.fromMST();
    // writing to file
    tsp.toFile("output.txt");
    return 0;
}
```

output.txt (resulting tour)
```
0
78934
111804
52086
18295
...
```

## References and inspirations

1. Bowyer-Watson algorithm and improvements: http://www.gdmc.nl/publications/2002/Bowyer_Watson_algorithm.pdf
2. O(nlogn) Euristic for Eucledian TSP: https://www.researchgate.net/publication/215753374_An_On_log_n_Heuristic_for_the_Euclidean_Traveling_Salesman_Problem
3. Images are generated from a dataset from [this](https://www.kaggle.com/c/traveling-santa-2018-prime-paths) Kaggle competition.
