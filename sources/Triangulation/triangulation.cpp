#include "triangulation.h"
#include "../Geometry/geometry.h"
#include <bits/stdc++.h>

using namespace std;

BowyerWatson::BowyerWatson() {}

BowyerWatson::BowyerWatson(vector<Point> points) {
    this->points = points;
}

void BowyerWatson::fromCoords(string path) {
    ifstream fin(path.c_str());
    ld a, b;
    int cnt = 0;
    while (fin >> a >> b)
        points.push_back(Point({cnt++, a, b}));
    cout << "finished reading" << endl;
    fin.close();
}

Triangle BowyerWatson::bigTriangle() {
    // Calculate the box (containing all the points) coordinates
    ld minx = inf, maxx = -inf, miny = inf, maxy = -inf;
    for (Point p : points) {
        minx = min(minx, p.x);
        maxx = max(maxx, p.x);
        miny = min(miny, p.y);
        maxy = max(maxy, p.y);
    }
    // Calculate the big triangle that contains all the points
    Triangle big = Triangle({-1, minx-1, miny-1},
                            {-2, maxx+maxy-miny+1, miny-0.1},
                            {-3, minx-0.1, maxy+maxx-minx+1});
    return big;
}

struct Node {
    int new_triangle;
    ld key;
};

bool operator<(Node a, Node b) {
    return a.key < b.key;
}

bool operator==(Node a, Node b) {
    return a.new_triangle == b.new_triangle;
}

inline ld genKey(Point centre, Point P) {
    Point diff = P-centre;
    ld angle = atan2(diff.y, diff.x);
    return (angle > 0)? angle : 2*PI+angle;
}

void BowyerWatson::compute() {
    // Bowyer-Watson algorithm for calculating the Delaunay Triangulation

    sort(points.begin(), points.end());
    triangles.push_back(bigTriangle());
    int active = 0;
    int progress = 0;
    for (Point p : points) {// iterate over the points, inserting one at a time
        //find a triangle that interferes with the point p
        queue<int> Q;
        Q.push(active);
        unordered_map<int, bool> used;
        bool finished = false;
        while(!triangles[active].inCircumcircle(p) && !finished) {// && !Q.empty()) {
            Triangle t = triangles[Q.front()];
            used[Q.front()] = true;
            Q.pop();
            for (int neigh : {t.a, t.b, t.c})
                if (neigh != -1 && !used[neigh]) {
                    if (triangles[neigh].inCircumcircle(p)) {
                        active = neigh;
                        finished = true;
                    }
                    Q.push(neigh);
                }
        }
        // Create an outer edge of the distortion
        set<Node> new_tri;
        queue<int> bad;
        bad.push(active);
        unordered_map<int, bool> visited;
        visited[active] = true;
        while(!bad.empty()) {
            int t_num = bad.front();
            triangles[t_num].dead = true;
            Triangle t = triangles[t_num];
            bad.pop();
            if (t.contains(p))
                active = triangles.size();
            // if there is no neighbor, create a triangle with an empty 'outer' neighbor
            if (t.a == -1) {
                Triangle new_triangle = Triangle(p, t.B, t.C);
                Point mid_point = {0, (t.B.x+t.C.x)/2, (t.B.y+t.C.y)/2};
                new_triangle.makeClockwise();
                new_tri.insert({triangles.size(), genKey(p, mid_point)});
                triangles.push_back(new_triangle);
            }
            if (t.b == -1) {
                Triangle new_triangle = Triangle(p, t.A, t.C);
                Point mid_point = {0, (t.A.x+t.C.x)/2, (t.A.y+t.C.y)/2};
                new_triangle.makeClockwise();
                new_tri.insert({triangles.size(), genKey(p, mid_point)});
                triangles.push_back(new_triangle);
            }
            if (t.c == -1) {
                Triangle new_triangle = Triangle(p, t.B, t.A);
                Point mid_point = {0, (t.B.x+t.A.x)/2, (t.B.y+t.A.y)/2};
                new_triangle.makeClockwise();
                new_tri.insert({triangles.size(), genKey(p, mid_point)});
                triangles.push_back(new_triangle);
            }
            for (int neigh_num : {t.a, t.b, t.c})
                if (neigh_num != -1 && !visited[neigh_num]) {
                    Triangle neigh = triangles[neigh_num];
                    if (neigh.inCircumcircle(p)) {
                        bad.push(neigh_num);
                        visited[neigh_num] = true;
                    } else {
                        // 1. create a new triangle with one vertex (A) in the point p and
                        //    the opposite edge (BC) being the common edge with the neighbor
                        // 2. make its points (A, B, C) go in a clockwise order
                        // 3. reassign the neighbor of neigh to new_triangle and vise versa
                        Triangle new_triangle;
                        if (neigh.a == t_num) {
                            new_triangle = Triangle(p, neigh.B, neigh.C);
                            new_triangle.makeClockwise();
                            new_triangle.a = neigh_num;
                            triangles[neigh_num].a = triangles.size();
                        } else if (neigh.b == t_num) {
                            new_triangle = Triangle(p, neigh.A, neigh.C);
                            new_triangle.makeClockwise();
                            new_triangle.a = neigh_num;
                            triangles[neigh_num].b = triangles.size();
                        } else {
                            new_triangle = Triangle(p, neigh.B, neigh.A);
                            new_triangle.makeClockwise();
                            new_triangle.a = neigh_num;
                            triangles[neigh_num].c = triangles.size();
                        }
                        // create a midpoint of the common edge
                        Point mid_point = {0, (new_triangle.B.x+new_triangle.C.x)/2,
                                              (new_triangle.B.y+new_triangle.C.y)/2};
                        new_tri.insert({triangles.size(), genKey(p, mid_point)});
                        triangles.push_back(new_triangle);
                    }
                }
        }
        // assign the 'side' neighbors of the new triangles
        set<Node>::iterator prev = new_tri.begin(), cur = new_tri.begin(), nxt = new_tri.begin();
        cur++;
        nxt++; nxt++;
        while(nxt != new_tri.end()) {
            int prev_tri = prev->new_triangle;
            int cur_tri = cur->new_triangle;
            int nxt_tri = nxt->new_triangle;
            triangles[cur_tri].b = prev_tri;
            triangles[prev_tri].c = cur_tri;
            triangles[cur_tri].c = nxt_tri;
            triangles[nxt_tri].b = cur_tri;
            // increment the pointers
            prev++; cur++; nxt++;
        }
        int first = new_tri.begin()->new_triangle;
        int last = cur->new_triangle;
        triangles[first].b = last;
        triangles[last].c = first;

        // show progress
        if (progress++%1000 == 0) {
            system("cls");
            cout << "building triangulation: " << progress*100.0/points.size() << "%\n";
        }
    }
    vector<Triangle> res;
    for (Triangle t : triangles)
        if (!t.dead && t.A.n >= 0 && t.B.n >= 0 && t.C.n >= 0)
                res.push_back(t);
    triangles = res;
    cout << "Built " << triangles.size() << "-triangle Triangulation" << endl;
}

void BowyerWatson::showStruct() {
    cout << "STRUCT:" << endl;
    queue<int> QU;
    QU.push(triangles.size()-1);
    unordered_map<int, bool> uused;
    uused[triangles.size()-1] = true;
    while(!QU.empty()) {
        Triangle t = triangles[QU.front()];
        QU.pop();
        cout << "{" << t.A.n << ", " << t.B.n << ", " << t.C.n << "}\n";
        if (t.a != -1)
            cout << "neigh: {" << triangles[t.a].A.n << ", " <<
                                  triangles[t.a].B.n << ", " <<
                                  triangles[t.a].C.n << "}\n";
        else
            cout << "neigh: NONE" << endl;
        if (t.b)
            cout << "       {" << triangles[t.b].A.n << ", " <<
                                  triangles[t.b].B.n << ", " <<
                                  triangles[t.b].C.n << "}\n";
        else
            cout << "       NONE" << endl;
        if (t.c)
            cout << "       {" << triangles[t.c].A.n << ", " <<
                                  triangles[t.c].B.n << ", " <<
                                  triangles[t.c].C.n << "}\n";
        else
            cout << "       NONE" << endl;
        for (int neigh : {t.a, t.b, t.c})
            if (neigh != -1 && !uused[neigh]) {
                QU.push(neigh);
                uused[neigh] = true;
            }
    }
}

void BowyerWatson::toEdges(string path) {
    set<Edge> res;
    for (Triangle t : triangles) {
        res.insert(t.AB);
        res.insert(t.AC);
        res.insert(t.BC);
    }
    ofstream fout(path.c_str());
    for (auto i = res.begin(); i != res.end(); i++)
        fout << i->A.n << " " << i->B.n << endl;
    fout.close();
}

void BowyerWatson::toTriangles(string path) {
    ofstream fout(path.c_str());
    for (Triangle t : triangles)
        fout << t.A.n << " " << t.B.n << " " << t.C.n << endl;
    fout.close();
}

vector<Point> BowyerWatson::pick(int n) {
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
