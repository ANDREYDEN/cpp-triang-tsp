#include "tsp.h"
#include <bits/stdc++.h>

// DSU

DSU::DSU(int V) {
    parent.resize(V);
    for (int i = 0; i < V; i++)
        parent[i] = i;
}

int DSU::top(int n) {
    if (parent[n] == n)
        return n;
    else
        return parent[n] = top(parent[n]);
}

void DSU::connect(Edge edge) {
    parent[top(edge.A.n)] = top(edge.B.n);
}

// TSP

TSP::TSP() {}

TSP::TSP(vector<Point> points) {
    this->points = points;
}

TSP::TSP(vector<Edge> edges) {
    this->edges = edges;
    for (Edge edge : edges) {
        points.push_back(edge.A);
        points.push_back(edge.B);
    }
}

void TSP::stripPoints(int points_in_strip) {
    // stripping points and 'sawing' the strips up
    sort(points.begin(), points.end());
    for (int j = 0; j < points_in_strip; j++)
        for (int k = j+1; k < points_in_strip; k++)
            edges.push_back(Edge(points[j], points[k]));

    for (int i = points_in_strip; i < points.size(); i += points_in_strip) {
        for (int j = i; j < i + points_in_strip && j < points.size(); j++)
            for (int k = j+1; k < i + points_in_strip && k < points.size(); k++)
                edges.push_back(Edge(points[j], points[k]));

        for (int j = i-points_in_strip; j < i; j++)
            for (int k = i; k < i + points_in_strip && k < points.size(); k++)
                edges.push_back(Edge(points[j], points[k]));
    }
    cout << "Stripped" << endl;
}

void TSP::toMST() {
    // create Minimal Spanning tree from the input edges
    sort(edges.begin(), edges.end());
    vector<Edge> res;
    DSU dsu = DSU(points.size());
    for (Edge edge : edges)
        if(dsu.top(edge.A.n) != dsu.top(edge.B.n)) {
            dsu.connect(edge);
            res.push_back(edge);
        }
    edges = res;
}

void TSP::fromMST() {
    // compute the tour based on the MST
    // make sure that 'edges' contain the edges of the MST
    // you may use the 'toMST' function
    vector<vector<int>> adj(points.size());
    for (Edge edge : edges) {
        adj[edge.A.n].push_back(edge.B.n);
        adj[edge.B.n].push_back(edge.A.n);
    }
    int st = 0;
    result.push_back(st);
    vector<bool> used(points.size(), false);
    stack<int> seq;
    seq.push(st);
    cout << "done" << endl;
    while (!seq.empty()) {
        int cur = seq.top();
        used[cur] = true;
        bool jump = false;
        for (int u : adj[cur])
            if (!used[u]) {
                seq.push(u);
                result.push_back(u);
                jump = true;
                break;
            }
        if (!jump)
            seq.pop();
    }
    result.push_back(0);
}

void TSP::CLA() {
    // Cheapest Link Algorithm
    sort(edges.begin(), edges.end());
    vector<vector<int>> nearest(points.size());
    for (Edge e : edges) {
        nearest[e.A.n].push_back(e.B.n);
        nearest[e.B.n].push_back(e.A.n);
    }

    result = {0};
    int smallest_free = 1;
    vector<bool> used(points.size(), false);
    used[0] = true;
    while (smallest_free != points.size()) {
        bool dead_end = true;
        for (int neigh : nearest[result.back()])
            if (!used[neigh]) {
                used[neigh] = true;
                result.push_back(neigh);
                if (result.back() == smallest_free)
                    while (smallest_free < points.size() && used[smallest_free])
                        smallest_free++;
                dead_end = false;
                break;
            }
        if (dead_end) {
            used[smallest_free] = true;
            result.push_back(smallest_free);
            while (smallest_free < points.size() && used[smallest_free])
                smallest_free++;
        }
    }
    result.push_back(0);
}

void TSP::toSmallestEdges() {
    // Sort the edges
    // add edges starting from the smallest
    // stop when the graph becomes connected
    sort(edges.begin(), edges.end());
    vector<bool> free(points.size(), true);
    vector<Edge> res;
    int smallest_free = 0;
    for (Edge e : edges) {
        res.push_back(e);
        free[e.A.n] = free[e.B.n] = false;
        while (smallest_free < points.size() && !free[smallest_free])
            smallest_free++;
        if (smallest_free >= points.size())
            break;
    }
    edges = res;
}

void TSP::toFile(string filename) {
    ofstream fout(filename.c_str());
    for (int e : result)
        fout << e << '\n';
    fout.close();
}
