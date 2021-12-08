#include <cfloat>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

struct Edge {
    int from, to;
    vector<pair<double, double>> weights;

    Edge(int from, int to, vector<pair<double, double>> weights)
        : from(from), to(to), weights(weights) {}

    double weight() {
        double r = rand() % (int)1e9 / 1e9;
        for (auto p : weights) {
            r -= p.second;
            if (r < 0) return p.first;
        }
        return weights.back().first;
    }
};

struct Path {
    vector<int> vertexes;

    void print() const {
        for (int i = 0; i < (int)vertexes.size(); i++) {
            if (i) cout << " -> ";
            cout << vertexes[i];
        }
        cout << endl;
    }

    Path operator+=(int v) {
        vertexes.emplace_back(v);
        return *this;
    }

    Path operator+(int v) {
        Path res = *this;
        res += v;
        return res;
    }
};

bool operator<(const Path& lhs, const Path& rhs) {
    return lhs.vertexes < rhs.vertexes;
}

Path Dijkstra(vector<vector<Edge>>& G, int& s, int& t) {
    int V = G.size();
    int u = s;
    set<int> vis;
    vis.emplace(s);
    vector<Path> P(V);
    P[s] += s;
    vector<double> weight(V, DBL_MAX);
    weight[s] = 0;
    do {
        for (auto edge : G[u]) {
            int v = edge.to;
            if (vis.count(v)) continue;
            double w = edge.weight();
            if (weight[v] > weight[u] + w) {
                P[v] = P[u] + edge.to;
                weight[v] = weight[u] + w;
            }
        }
        double mn = DBL_MAX;
        for (int v = 0; v < V; v++) {
            if (vis.count(v)) continue;
            if (weight[v] < mn) {
                u = v;
                mn = weight[v];
            }
        }
        if (mn == DBL_MAX) break;
        vis.emplace(u);
    } while (u != t);
    return P[t];
}

map<Path, double> solve(vector<vector<Edge>>& G, int& s, int& t, int& m) {
    map<Path, double> cnt;
    for (int i = 0; i < m; i++) {
        auto P = Dijkstra(G, s, t);
        cnt[P] += 1.0 / m;
    }
    return cnt;
}

int main(int argc, char const* argv[]) {
    int m = atoi(argv[1]);
    int V, E, N;
    cin >> V >> E >> N;
    vector<vector<Edge>> G(V);
    for (int e = 0; e < E; e++) {
        int from, to;
        cin >> from >> to;
        vector<pair<double, double>> weights(N);
        for (int i = 0; i < N; i++) {
            cin >> weights[i].first >> weights[i].second;
        }
        G[from].emplace_back(from, to, weights);
    }
    int s, t;
    cin >> s >> t;
    auto cnt = solve(G, s, t, m);
    for (auto p : cnt) {
        cout << "probability : " << p.second << endl;
        cout << "path        : ";
        p.first.print();
    }
    return 0;
}