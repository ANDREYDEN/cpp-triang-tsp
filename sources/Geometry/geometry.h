#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include <bits/stdc++.h>

using namespace std;

typedef long double ld;

const int inf = 1e9+7;
const ld PI = acos(-1);
const ld EPS = 0.00000001;

struct Point {
    int n;
    ld x, y;

    ld dist(Point other);
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    bool operator<(const Point& other) const;
    Point operator-(const Point& other) const;
};

struct Edge {
    Point A, B;
    ld len;

    Edge();
    Edge(Point A, Point B);
    bool operator==(const Edge& other) const;
    bool operator<(const Edge& other) const;
};

struct EdgeHash {
    const size_t operator()(const Edge& e) const;
};

struct Triangle {
    Point A, B, C;
    Edge AB, BC, AC;
    int a, b, c; // numbers of neighbors
    bool dead; // determines if the triangle was excluded from the triangulation

    Triangle();
    Triangle(Point A, Point B, Point C);
    Point circumcircleCentre();
    ld circumcircleRad();
    bool inCircumcircle(Point P);
    bool contains(Point P);
    void makeClockwise();
    bool operator==(const Triangle& other) const;
};

struct TriHash {
    const size_t operator()(const Triangle& t) const;
};

vector<Edge> fullGraph(vector<Point> points);
vector<Point> pick(vector<Point> points, int n);
vector<Point> toPoints(string filename);
void fromPoints(string filename, vector<Point> pts);
vector<Edge> toEdges(string filename, vector<Point> pts, int num_of_vert);
void fromEdges(string filename, vector<Edge> edges);

#endif // GEOMETRY_H_INCLUDED
