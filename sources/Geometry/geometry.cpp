#include "geometry.h"
#include <bits/stdc++.h>

using namespace std;

//Point

ld Point::dist(Point other) {
    return sqrt((x - other.x)*(x - other.x) +
                (y - other.y)*(y - other.y));
}

bool Point::operator==(const Point& other) const {
    return this->n == other.n;
}

bool Point::operator!=(const Point& other) const {
    return this->n != other.n;
}

bool Point::operator<(const Point& other) const{
    return (abs(this->x - other.x) < 90)? this->y < other.y : this->x < other.x;
}

Point Point::operator-(const Point& other) const {
    return {-1, this->x-other.x, this->y-other.y};
}

//Edge

Edge::Edge() {}

Edge::Edge(Point A, Point B) {
    this->A = A;
    this->B = B;
    len = sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

bool Edge::operator==(const Edge& other) const {
    return (A == other.A && B == other.B) ||
           (A == other.B && B == other.A);
}

bool Edge::operator<(const Edge& other) const {
    return this->len < other.len;
}

const size_t EdgeHash::operator()(const Edge& e) const {
    return (e.A.n + size_t(e.B.n)*inf)%(inf+2);
}

//Triangle

Triangle::Triangle() {}

Triangle::Triangle(Point A, Point B, Point C) {
    this->A = A;
    this->B = B;
    this->C = C;
    AB = Edge(A, B);
    BC = Edge(B, C);
    AC = Edge(A, C);
    a = b = c = -1;
    dead = false;
}

Point Triangle::circumcircleCentre() {
    ld x1 = A.x, y1 = A.y;
    ld x2 = B.x, y2 = B.y;
    ld x3 = C.x, y3 = C.y;
    ld x = ((x1*x1+y1*y1)*(y3-y2) + (x2*x2+y2*y2)*(y1-y3) + (x3*x3+y3*y3)*(y2-y1))/2/
               (x1*(y3-y2) + x2*(y1-y3) + x3*(y2-y1));
    ld k = (x2 - x3)/(y3-y2);
    ld b = (x3*x3 + y3*y3 - x2*x2 - y2*y2)/2/(y3-y2);
    ld y = k*x + b;
    return {0, x, y};
}

ld Triangle::circumcircleRad() {
    ld aa = BC.len;
    ld bb = AC.len;
    ld cc = AB.len;
    ld p = (aa+bb+cc)/2;
    ld S = sqrt(p*(p-aa)*(p-bb)*(p-cc));
    return aa*bb*cc/4/S;
}

bool Triangle::inCircumcircle(Point P) {
    // checks if the surcumscribed circle of the triangle contains a point
    Point centre = circumcircleCentre();
    ld r = circumcircleRad();
    return P.dist(centre) < r;
}

bool Triangle::contains(Point P) {
    // checks if the triangle contains a point
    ld x1 = P.dist(A);
    ld x2 = P.dist(B);
    ld x3 = P.dist(C);
    ld aa = BC.len;
    ld bb = AC.len;
    ld cc = AB.len;
    ld a1 = acos((x2*x2+x3*x3-aa*aa)/2/x2/x3);
    ld a2 = acos((x1*x1+x3*x3-bb*bb)/2/x1/x3);
    ld a3 = acos((x2*x2+x1*x1-cc*cc)/2/x2/x1);
    return abs(a1+a2+a3-2*PI) < EPS;
}

void Triangle::makeClockwise() {
    // sorts vertices clockwise
    Point diffB = B - A;
    Point diffC = C - A;
    ld angleB = atan2(diffB.y, diffB.x);
    ld angleC = atan2(diffC.y, diffC.x);
    if ((angleB < 0 && angleC > 0 && angleC - angleB < PI) ||
        (angleB > 0 && angleC < 0 && angleB - angleC > PI) ||
        (angleB < 0 && angleC < 0 && angleC > angleB) ||
        (angleB > 0 && angleC > 0 && angleC > angleB)) {
        swap(B, C);
        swap(b, c);
        swap(AC, AB);
    }
}

bool Triangle::operator==(const Triangle& other) const {
    // will only compare correctly if the triangles are preordered clockwise
    return (this->A == other.A && this->B == other.B && this->C == other.C);
}

const size_t TriHash::operator()(const Triangle& t) const {
    return t.A.n*t.BC.len + t.B.n*t.AC.len + t.C.n*t.AB.len;
}

// Other Functions

vector<Edge> fullGraph(vector<Point> points) {
    vector<Edge> edges;
    for (int i = 0; i < points.size(); i++)
        for (int j = i+1; j < points.size(); j++)
            edges.push_back(Edge(points[i], points[j]));
    return edges;
}

vector<Point> pick(vector<Point> points, int n) {
    srand(time(0));
    vector<Point> res(n);
    int i;
    for (i = 0; i < n; i++)
        res[i] = points[i];
    for (; i < points.size(); i++) {
        int j = rand()%(i+1);
        if (j < n)
            res[j] = points[i];
    }
    return res;
}

vector<Point> toPoints(string filename) {
    ifstream fin(filename.c_str());
    vector<Point> res;
    ld x, y;
    int cnt = 0;
    while (fin >> x >> y)
        res.push_back({cnt++, x, y});
    fin.close();
    return res;
}

vector<Edge> toEdges(string filename, vector<Point> pts, int num_of_vert) {
    ifstream fin(filename.c_str());
    unordered_set<Edge, EdgeHash> res;
    int first;
    while (fin >> first) {
        int prev = first, cur;
        for (int i = 0; i < num_of_vert; i++) {
            fin >> cur;
            res.insert(Edge(pts[prev], pts[cur]));
            prev = cur;
        }
        if (num_of_vert > 2)
            res.insert(Edge(pts[cur], pts[first]));
    }
    fin.close();
    vector<Edge> unrepeating;
    for (unordered_set<Edge, EdgeHash>::iterator i = res.begin(); i != res.end(); i++)
        unrepeating.push_back(*i);
    return unrepeating;
}

void fromEdges(string filename, vector<Edge> edges) {
    ofstream fout(filename.c_str());
    for (Edge edge : edges)
        fout << edge.A.n << " " << edge.B.n << '\n';
    fout.close();
}
