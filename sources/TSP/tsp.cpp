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
    // for each point create a set of its neighbors sorted anticlockwise
    vector<set<pair<ld, Point>>> adj(points.size());
    for (Edge e : edges) {
        adj[e.A.n].insert({genKey(e.A, e.B), e.B});
        adj[e.B.n].insert({genKey(e.B, e.A), e.A});
    }
//    for (int i = 0; i < points.size(); i++) {
//        cout << i << ": ";
//        for (auto j = adj[i].begin(); j != adj[i].end(); j++)
//            cout << j->second.n << " ";
//        cout << endl;
//    }
    set<Edge> test(edges.begin(), edges.end());
    estruct res;
    for (Edge e : edges)
        res[e] = {{-1, -1, -1}, {-1, -1, -1}};
    for (Point P : points) {
        //cout << P.n << ": ";
        auto st = adj[P.n].begin();
        auto cur = st, nxt = st;
        nxt++;
        Edge e;
        for (;nxt != adj[P.n].end(); cur++, nxt++) {
            e = Edge(cur->second, nxt->second);
            //cout << "{" << e.A.n << ", " << e.B.n << "} ";
            if (res[e].first.n == -1)
                res[e].first = P;
            else
                res[e].second = P;
        }
        if (adj[P.n].size() > 2) {
            e = Edge(cur->second, st->second);
            //cout << "{" << e.A.n << ", " << e.B.n << "}\n";
            if (res[e].first.n == -1)
                res[e].first = P;
            else
                res[e].second = P;
        }
    }
//    for (Edge e : edges) {
//        cout << e.A.n << " " << e.B.n << " | " << res[e].first.n << " " << res[e].second.n << endl;
//    }
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
//    for (int i = 0; i < points.size(); i++)
//        if (adj[i].size() == 1)
//            cout << i << " " << adj[i][0] << " " << adj[i][1] << endl;
    result.push_back(0);
    int cur = adj[0][0];
    int prev = 0;
    while (cur != 0) {
        result.push_back(cur);
        int temp = cur;
        //cout << cur << " " << adj[cur][0] << " " << adj[cur][1] << endl;
        cur = (adj[cur][0] != prev)? adj[cur][0] : adj[cur][1];
        prev = temp;
    }
    result.push_back(0);
}

void TSP::triangleBFS() {
    // Iteratively adding points to the TSP tour
    Edge stEdge;
    ld minLen = inf;
    for (Edge e : edges)
        if ((e.A.n == 0 || e.B.n == 0) && e.len < minLen) {
            stEdge = e;
            minLen = e.len;
        }
    vector<int> to(points.size(), -1);
    estruct adj = edgeStruct();
    cout << "made edge structure" << endl;
    queue<Edge> Q;
    vector<int> used(points.size(), false);
    used[stEdge.A.n] = used[stEdge.B.n] = true;
    to[stEdge.A.n] = stEdge.B.n;
    to[stEdge.B.n] = stEdge.A.n;
    Q.push(stEdge);
    int cnt;
    while (!Q.empty()) {
        Edge cur = Q.front();
//        cout << 0 << " ";
//        int temp = to[0];
//        while (temp != 0) {
//            cout << temp << " ";
//            temp = to[temp];
//        }
//        cout << temp << endl;
        Q.pop();
        //cout << cur.A.n << " " << cur.B.n << " | " << adj[cur].first.n << " " << adj[cur].second.n << endl;
        if (cur.A.dist(adj[cur].first) + cur.B.dist(adj[cur].first) >
            cur.A.dist(adj[cur].second) + cur.B.dist(adj[cur].second))
            swap(adj[cur].first, adj[cur].second);
        if (!used[adj[cur].first.n]) {
            used[adj[cur].first.n] = true;

            if (to[cur.A.n] == cur.B.n) {
                to[cur.A.n] = adj[cur].first.n;
                to[adj[cur].first.n] = cur.B.n;
            } else {
                to[cur.B.n] = adj[cur].first.n;
                to[adj[cur].first.n] = cur.A.n;
            }

            Q.push(Edge(cur.A, adj[cur].first));
            Q.push(Edge(cur.B, adj[cur].first));
        } else if (!used[adj[cur].second.n]){
            used[adj[cur].second.n] = true;

            if (to[cur.A.n] == cur.B.n) {
                to[cur.A.n] = adj[cur].second.n;
                to[adj[cur].second.n] = cur.B.n;
            } else {
                to[cur.B.n] = adj[cur].second.n;
                to[adj[cur].second.n] = cur.A.n;
            }

            Q.push(Edge(cur.A, adj[cur].second));
            Q.push(Edge(cur.B, adj[cur].second));
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
        //cout << cur << endl;
        result.push_back(cur);
        cur = to[cur];
    }
    cout << "points :" << points.size() << endl;
    cout << "result size: " << result.size() << endl;
    result.push_back(0);
}

void TSP::toFile(string filename) {
    ofstream fout(filename.c_str());
    for (int e : result)
        fout << e << '\n';
    fout.close();
}
