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
    // Traveling Salesman Problem algorithms for finding solutions in large pointsets
    public :
        TSP();
        TSP(vector<Point> coords);
        TSP(vector<Edge> edges);
        // points -> edges
        void stripPoints(int points_in_strip);
        void toMST();
        void toSmallestEdges();
        // edges -> result
        void fromMST();
        void CLA();
        void triangleBFS();
        // other
        void toFile(string filename);
        ld tourLength(vector<Point> coords);
        ld tourPrimeLength(vector<Point> coords);
    private :
        vector<Point> points;
        vector<Edge> edges;
        vector<int> result;
        // the result contains the tour
        // (numbers of points according to the entered sequence) beginning and ending at 0

        // edges -> edge data structure
        unordered_map<Edge, pair<Point, Point>, EdgeHash> edgeStruct();
        // quality and rank of an edge
        ld quality(Edge e, Point L, Point R, int rnk);
        unordered_map<Edge, int, EdgeHash> ranking();
};

#endif // TSP_H_INCLUDED
