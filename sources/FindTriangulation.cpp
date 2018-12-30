#include <bits/stdc++.h>
#include "Triangulation/triangulation.h"
#include "Geometry/geometry.h"

using namespace std;

int main() {
    vector<Point> test = {{0, 1, 3},
                          {1, 2, 7},
                          {2, 3, 4},
                          {3, 4, 2},
                          {4, 5, 9},
                          {5, 6, 5},
                          {6, 7, 3},
                          {7, 8, 7},
                          {8, 9, 8}};
    BowyerWatson bw;// = BowyerWatson(test);
    bw.fromCoords("input.txt");
    bw.compute();
    bw.toEdges("triang.txt");
    //bw.toEdges("all/temp.txt");
}
