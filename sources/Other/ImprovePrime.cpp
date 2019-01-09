#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;

const int V = 197769;

struct Point {
    int n;
    double x, y;

    inline double dist(Point other) {
        return sqrt((this->x - other.x)*(this->x - other.x) +
                    (this->y - other.y)*(this->y - other.y));
    }
};

vector<bool> P;
vector<Point> coords;
vector<int> fact = {1, 1, 2, 6, 24, 120, 720, 5040};

vector<bool> genPrimes(int N) {
    vector<bool> prime(N, true);
    prime[0] = prime[1] = false;
    for (int i = 2; i < N; i++)
        if (prime[i])
            for (int j = i+i; j < N; j+=i)
                prime[j] = false;
    return prime;
}

double len(vector<int> seq, int start_ind=0, bool closed=false) {
    double res = 0;
    for (int i = 0; i < seq.size()-1; i++)
        if ((start_ind+i)%10 == 9 && !P[seq[i]])
            res += coords[seq[i]].dist(coords[seq[i+1]])*1.1;
        else
            res += coords[seq[i]].dist(coords[seq[i+1]]);
    if (closed)
        res += coords[seq.back()].dist(coords[seq[0]]);
    return res;
}

vector<int> improve(vector<int> path, int seglen) {
    vector<int> segment, best, new_path;
    int i = 0;
    for (; i+seglen-1 < path.size(); i += seglen-1) {
        if (i%10000 == 0) cout << i << endl;
        segment.clear();
        copy(path.begin()+i, path.begin()+i+seglen, back_inserter(segment));
        best = segment;
        double bestlen = len(segment, i);
        for (int k = 0; k < fact[seglen-2]-1; k++) {
            next_permutation(segment.begin()+1, segment.end()-1);
            double curlen = len(segment, i);
            if (curlen < bestlen) {
                bestlen = curlen;
                best = segment;
            }
        }
        copy(best.begin(), best.end()-1, back_inserter(new_path));
    }
    for (; i < path.size(); i++)
        new_path.push_back(path[i]);
    return new_path;
}

vector<int> repImprove(vector<int> path, int n, int lower, int upper) {
    srand(time(0));
    while (n--) {
        int k = rand()%(upper-lower+1) + lower;
        path = improve(path, k);
        cout << "After" << k << ": (" << path.size() << ") " << len(path, 0, true) << endl;
    }
    return path;
}

int main() {
    ifstream fin("input.txt");
    for (int i = 0; i < V; i++) {
        double a, b;
        fin >> a >> b;
        coords.push_back(Point({i, a, b}));
    }
    cout << "finished reading coords" << endl;
    fin.close();

    fin.open("output.txt");
    vector<int> path(V+1);
    for (int i = 0; i <= V; i++)
        fin >> path[i];
    cout << "finished reading path" << endl;
    fin.close();

    P = genPrimes(V);
    cout << "Before: (" << path.size() << ") " << len(path, 0, true) << endl;
    //path = repImprove(path, 1, 9, 9);
    path = repImprove(path, 10, 4, 8);

    cout << "Proceed? (y/n)";
    string temp;
    cin >> temp;
    if (temp == "y") {
        ofstream fout("result.txt");
        for (int v : path)
            fout << v << '\n';
        fout.close();
    }
}
