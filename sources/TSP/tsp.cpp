#include "tsp.h"
#include <bits/stdc++.h>

typedef unordered_map<Edge, pair<Point, Point>, EdgeHash> estruct;

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
    unordered_map<int, bool> used;
    for (Edge edge : edges) {
        if (!used[edge.A.n]) {
            points.push_back(edge.A);
            used[edge.A.n] = true;
        }
        if (!used[edge.B.n]) {
            points.push_back(edge.B);
            used[edge.B.n] = true;
        }
    }
}

inline ld genKey(Point centre, Point P) {
    Point diff = P-centre;
    ld angle = atan2(diff.y, diff.x);
    return (angle > 0)? angle : 2*PI+angle;
}

estruct TSP::edgeStruct() {
    // For each point create a set of its neighbors sorted anticlockwise
    vector<set<pair<ld, Point>>> adj(points.size());
    for (Edge e : edges) {
        adj[e.A.n].insert({genKey(e.A, e.B), e.B});
        adj[e.B.n].insert({genKey(e.B, e.A), e.A});
    }
    set<Edge> test(edges.begin(), edges.end());
    estruct res;
    for (Edge e : edges)
        res[e] = {{-1, inf, inf}, {-1, inf, inf}};
    for (Point P : points) {
        auto st = adj[P.n].begin();
        auto cur = st, nxt = st;
        nxt++;
        Edge e;
        for (;nxt != adj[P.n].end(); cur++, nxt++) {
            e = Edge(cur->second, nxt->second);
            if (res[e].first.n == -1)
                res[e].first = P;
            else
                res[e].second = P;
        }
        if (adj[P.n].size() > 2) {
            e = Edge(cur->second, st->second);
            if (res[e].first.n == -1)
                res[e].first = P;
            else
                res[e].second = P;
        }
    }
    return res;
}

unordered_map<Edge, int, EdgeHash> TSP::ranking() {
    // give each edge a rank
    vector<set<pair<ld, Point>>> adj(points.size());
    for (Edge e : edges) {
        adj[e.A.n].insert({e.A.dist(e.B), e.B});
        adj[e.B.n].insert({e.A.dist(e.B), e.A});
    }
    unordered_map<Edge, int, EdgeHash> res;
    for (Point P : points) {
        auto neighbors = adj[P.n];
        int cnt = 0;
        for (auto i = neighbors.begin(); i != neighbors.end(); i++) {
            Edge e = Edge(P, i->second);
            res[e] += cnt++;
        }
    }
    return res;
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
    // Cheapest Link Algorithm ('edges' must contain full graph)
    sort(edges.begin(), edges.end());
    vector<vector<int>> adj(points.size());
    for (Edge edge : edges)
        if (adj[edge.A.n].size() + adj[edge.B.n].size() < 2) {
            adj[edge.A.n].push_back(edge.B.n);
            adj[edge.B.n].push_back(edge.A.n);
        }
    result.push_back(0);
    int cur = adj[0][0];
    int prev = 0;
    while (cur != 0) {
        result.push_back(cur);
        int temp = cur;
        cur = (adj[cur][0] != prev)? adj[cur][0] : adj[cur][1];
        prev = temp;
    }
    result.push_back(0);
}

void TSP::triangleBFS() {
    // Iteratively adding points to the TSP tour

    // find a staring edge (minimal length containing 0)
    Edge stEdge;
    ld minLen = inf;
    for (Edge e : edges)
        if (e.len < minLen) {
            stEdge = e;
            minLen = e.len;
        }
    vector<int> to(points.size(), -1); // pointer to the next point in the tour
    estruct adj = edgeStruct(); // edge -> {firstNeighbor, secondNeighbor}
    unordered_map<Edge, int, EdgeHash> rnk = ranking();
    cout << "made edge structure" << endl;
    priority_queue<pair<ld, Edge>> Q;
    vector<int> used(points.size(), false);
    used[stEdge.A.n] = used[stEdge.B.n] = true;
    to[stEdge.A.n] = stEdge.B.n;
    to[stEdge.B.n] = stEdge.A.n;
    Q.push({0, stEdge});
    int cnt;
    while (!Q.empty()) {
        Edge cur = Q.top().second;
        Q.pop();
        // order th neighbors {shorter, longer}
        if (cur.A.dist(adj[cur].first) + cur.B.dist(adj[cur].first) >
            cur.A.dist(adj[cur].second) + cur.B.dist(adj[cur].second))
            swap(adj[cur].first, adj[cur].second);

        Point neigh = {-1, -1, -1};
        if (!used[adj[cur].first.n]) {
            used[adj[cur].first.n] = true;
            neigh = adj[cur].first;
        } else if (!used[adj[cur].second.n]){
            used[adj[cur].second.n] = true;
            neigh = adj[cur].second;
        }
        if (neigh.n != -1) {
            if (to[cur.A.n] == cur.B.n) {
                to[cur.A.n] = neigh.n;
                to[neigh.n] = cur.B.n;
            } else {
                to[cur.B.n] = neigh.n;
                to[neigh.n] = cur.A.n;
            }
            Edge e1 = Edge(cur.A, neigh);
            Edge e2 = Edge(cur.B, neigh);

            Q.push({-quality(e1, adj[e1].first, adj[e1].second, rnk[e1]), e1});
            Q.push({-quality(e2, adj[e2].first, adj[e2].second, rnk[e2]), e2});
        }
    }

    // adding standalone points
    cout << "finishing..." << endl;
    vector<unordered_set<int>> neighbors(points.size());
    for (Edge e : edges) {
        neighbors[e.A.n].insert(e.B.n);
        neighbors[e.B.n].insert(e.A.n);
    }
    for (Point P : points)
        if (to[P.n] == -1) {
            int cur = to[0];
            while (cur != 0) {
                if (neighbors[P.n].find(cur) != neighbors[P.n].end()) break;
                cur = to[cur];
            }
            to[P.n] = to[cur];
            to[cur] = P.n;
        }

    int cur = to[0];
    result.push_back(0);
    while (cur != 0) {
        result.push_back(cur);
        cur = to[cur];
    }
    result.push_back(0);
}

ld TSP::quality(Edge e, Point L, Point R, int rnk) {
    if (L.n == -1)
        return e.A.dist(R) + e.B.dist(R) - e.len*rnk;
    else
        return e.A.dist(L) + e.B.dist(L) - e.len*rnk;
}

ld TSP::tourLength(vector<Point> coords) {
    // returns the tour length
    ld res = 0;
    for (int i = 0; i < result.size()-1; i++)
        res += coords[result[i]].dist(coords[result[i+1]]);
    res += coords[result.back()].dist(coords[result[0]]);
    return res;
}

ld TSP::tourPrimeLength(vector<Point> coords) {
    // Here each 10th edge is 1.1 times longer unless coming from a prime point
    vector<bool> prime(points.size(), true);
    prime[0] = prime[1] = false;
    for (int i = 2; i < points.size(); i++)
        if (prime[i])
            for (int j = i+i; j < points.size(); j+=i)
                prime[j] = false;

    ld res = 0;
    for (int i = 0; i < result.size()-1; i++)
        if (i%10 == 9 && !prime[result[i]])
            res += coords[result[i]].dist(coords[result[i+1]])*1.1;
        else
            res += coords[result[i]].dist(coords[result[i+1]]);
    res += coords[result.back()].dist(coords[result[0]]);
    return res;
}

void TSP::toFile(string filename) {
    ofstream fout(filename.c_str());
    for (int e : result)
        fout << e << '\n';
    fout.close();
}
