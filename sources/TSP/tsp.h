#ifndef TSP_H_INCLUDED
#define TSP_H_INCLUDED

#include "../Geometry/geometry.h"
#include <bits/stdc++.h>

struct DSU {
    // Disjoint Set Union structure and methods
    vector<int> parent;

    DSU(int V);
    int top(int n);
    void connect(Edge edge);
};

class TSP {
    // Traveling Salesman Problem algorithms for finding solutions
    // in large pointsets
    public :
        TSP();
        TSP(vector<Point> coords);
        TSP(vector<Edge> edges);
        void stripPoints(int points_in_strip);
        void toMST();
        void fromMST();
        void CLA();
        void toSmallestEdges();
        void toFile(string filename);
    private :
        vector<Point> points;
        vector<Edge> edges;
        vector<int> result;
        // the result contains the tour
        // (numbers of points according to the entered sequence)
        // beginning and ending at 0
};

#endif // TSP_H_INCLUDED
