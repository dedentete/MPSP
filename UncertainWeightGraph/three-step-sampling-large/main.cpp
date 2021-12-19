#include <algorithm>
#include <cfloat>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

struct Edge {
    int from, to;
    vector<double> weights, probabilities;

    Edge(int from, int to, vector<double> weights, vector<double> probabilities)
        : from(from), to(to), weights(weights), probabilities(probabilities) {}

    bool empty() {
        return weights.empty();
    }

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

    int size() {
        return vertexes.size();
    }

    void reverse() {
        std::reverse(vertexes.begin(), vertexes.end());
    }

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

    bool operator==(const Path& p) const {
        return vertexes == p.vertexes;
    }

    bool operator<(const Path& p) const {
        return vertexes < p.vertexes;
    }

    int operator[](size_t n) {
        return vertexes[n];
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

vector<double> PathSampling(vector<vector<Edge>>& G, Path& P) {
    vector<double> res;
    for (int i = 0; i + 1 < (int)P.size(); i++) {
        int u = P[i], v = P[i + 1];
        for (auto edge : G[u]) {
            if (edge.to == v) {
                res.emplace_back(edge.weight());
            }
        }
    }
    return res;
}

vector<vector<pair<int, double>>> GraphSampling(vector<vector<Edge>>& G,
                                                Path& P,
                                                vector<double> weights) {
    int V = G.size();
    vector<vector<pair<int, double>>> g(V);
    set<pair<int, int>> fixed;
    for (int i = 0; i + 1 < (int)P.size(); i++) {
        int u = P[i], v = P[i + 1];
        g[u].emplace_back(v, weights[i]);
        fixed.emplace(u, v);
    }
    for (int v = 0; v < V; v++) {
        for (auto edge : G[v]) {
            if (fixed.count(pair<int, int>(v, edge.to))) continue;
            g[v].emplace_back(edge.to, edge.weight());
        }
    }
    return g;
}

Path Dijkstra(vector<vector<pair<int, double>>>& g, int s, int t) {
    using P = pair<double, int>;
    int V = g.size();
    vector<double> weight(V, DBL_MAX);
    priority_queue<P, vector<P>, greater<P>> que;
    vector<int> prev(V, -1);
    weight[s] = 0;
    que.emplace(0, s);
    while (!que.empty()) {
        P p = que.top();
        que.pop();
        int v = p.second;
        if (weight[v] < p.first) continue;
        for (auto p : g[v]) {
            int to = p.first;
            if (weight[to] > weight[v] + p.second) {
                weight[to] = weight[v] + p.second;
                prev[to] = v;
                que.emplace(weight[to], to);
            }
        }
    }
    Path res;
    int cur = t;
    while (cur != -1) {
        res += cur;
        cur = prev[cur];
    }
    res.reverse();
    return res;
}

vector<pair<double, Path>> solve(vector<vector<Edge>>& G, int s, int t, int m,
                                 int N_P, int N_G) {
    set<Path> CP;
    for (int i = 0; i < m; i++) {
        auto P = DijkstraMC(G, s, t);
        CP.emplace(P);
    }
    vector<pair<double, Path>> res;
    for (auto P : CP) {
        map<vector<double>, double> cnt;
        for (int i = 0; i < N_P; i++) {
            cnt[PathSampling(G, P)] += 1.0 / N_P;
        }
        double probability = 0;
        for (auto p : cnt) {
            double shortest_probability = 0;
            for (int i = 0; i < N_G; i++) {
                auto g = GraphSampling(G, P, p.first);
                auto SP = Dijkstra(g, s, t);
                if (P == SP) {
                    shortest_probability += 1.0 / N_G;
                }
            }
            probability += p.second * shortest_probability;
        }
        res.emplace_back(probability, P);
    }
    sort(res.begin(), res.end());
    reverse(res.begin(), res.end());
    return res;
}

int main(int argc, char const* argv[]) {
    int m = atoi(argv[1]), N_P = atoi(argv[2]), N_G = atoi(argv[3]);
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
    auto ans = solve(G, s, t, m, N_P, N_G);
    for (auto ans : ans) {
        ans.second.print();
        cout << ans.first << endl;
    }
    return 0;
}