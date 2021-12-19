#include <algorithm>
#include <cfloat>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

struct Edge {
    int from, to;
    vector<double> weights, probabilities;

    Edge(int from, int to, vector<double> weights, vector<double> probabilities)
        : from(from), to(to), weights(weights), probabilities(probabilities) {}

    double weight() {
        double r = rand() % (int)1e9 / 1e9;
        for (int i = 0; i < (int)weights.size(); i++) {
            r -= probabilities[i];
            if (r < 0) return weights[i];
        }
        return weights.back();
    }
};

struct Path {
    vector<int> vertexes;

    void print() const {
        for (int i = 0; i < (int)vertexes.size(); i++) {
            if (i) cout << " ";
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

    bool operator<(const Path& p) const {
        return vertexes < p.vertexes;
    }
};

Path DijkstraMC(vector<vector<Edge>>& G, int s, int t) {
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
                P[v] = P[u] + v;
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

vector<pair<double, Path>> solve(vector<vector<Edge>>& G, int s, int t,
                                 int m) {
    map<Path, double> cnt;
    for (int i = 0; i < m; i++) {
        auto P = DijkstraMC(G, s, t);
        cnt[P] += 1.0 / m;
    }
    vector<pair<double, Path>> res;
    for (auto p : cnt) {
        res.emplace_back(p.second, p.first);
    }
    sort(res.begin(), res.end());
    reverse(res.begin(), res.end());
    return res;
}

int main(int argc, char const* argv[]) {
    int m = atoi(argv[1]);
    int V, E, N;
    cin >> V >> E >> N;
    vector<vector<Edge>> G(V);
    for (int e = 0; e < E; e++) {
        int from, to;
        cin >> from >> to;
        vector<double> weights(N), probabilities(N);
        for (int i = 0; i < N; i++) {
            cin >> weights[i] >> probabilities[i];
        }
        G[from].emplace_back(from, to, weights, probabilities);
    }
    int s, t;
    cin >> s >> t;
    auto ans = solve(G, s, t, m);
    for (auto ans : ans) {
        ans.second.print();
        cout << ans.first << endl;
    }
    return 0;
}