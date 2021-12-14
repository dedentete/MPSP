#include <algorithm>
#include <cfloat>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

struct Edge {
    vector<double> weights, probabilities;

    Edge() {}
    Edge(vector<double> weights, vector<double> probabilities)
        : weights(weights), probabilities(probabilities) {}

    bool empty() {
        return weights.empty();
    }

    double weight() {
        double r = rand() % (int)1e9 / 1e9;
        for (int i = 0; i < (int)weights.size(); i++) {
            r -= probabilities[i];
            if (r < 0) return weights[i];
        }
        return -1;
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
        for (int v = 0; v < V; v++) {
            if (G[u][v].empty()) continue;
            if (vis.count(v)) continue;
            double w = G[u][v].weight();
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
        res.emplace_back(G[u][v].weight());
    }
    return res;
}

vector<vector<double>> GraphSampling(vector<vector<Edge>>& G, Path& P,
                                     vector<double> weights) {
    int V = G.size();
    vector<vector<double>> g(V, vector<double>(V, -1));
    for (int i = 0; i + 1 < (int)P.size(); i++) {
        int u = P[i], v = P[i + 1];
        g[u][v] = weights[i];
    }
    for (int u = 0; u < V; u++) {
        for (int v = 0; v < V; v++) {
            if (g[u][v] != -1) continue;
            g[u][v] = G[u][v].weight();
        }
    }
    return g;
}

Path Dijkstra(vector<vector<double>>& g, int s, int t) {
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
        for (int to = 0; to < V; to++) {
            if (g[v][to] == -1) continue;
            if (weight[to] > weight[v] + g[v][to]) {
                weight[to] = weight[v] + g[v][to];
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

vector<pair<double, Path>> solve(vector<vector<Edge>>& G, int s, int t,
                                 int number_of_DijkstraMC,
                                 int number_of_PathSampling,
                                 int number_of_GraphSampling) {
    set<Path> CP;
    for (int i = 0; i < number_of_DijkstraMC; i++) {
        auto P = DijkstraMC(G, s, t);
        CP.emplace(P);
    }
    vector<pair<double, Path>> res;
    for (auto P : CP) {
        map<vector<double>, double> cnt;
        for (int i = 0; i < number_of_PathSampling; i++) {
            cnt[PathSampling(G, P)] += 1.0 / number_of_PathSampling;
        }
        double probability = 0;
        for (auto p : cnt) {
            double shortest_probability = 0;
            for (int i = 0; i < number_of_GraphSampling; i++) {
                auto g = GraphSampling(G, P, p.first);
                auto SP = Dijkstra(g, s, t);
                if (P == SP) {
                    shortest_probability += 1.0 / number_of_GraphSampling;
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
    int number_of_DijkstraMC = atoi(argv[1]),
        number_of_PathSampling = atoi(argv[2]),
        number_of_GraphSampling = atoi(argv[3]);
    int V, E, N;
    cin >> V >> E >> N;
    vector<vector<Edge>> G(V, vector<Edge>(V));
    for (int e = 0; e < E; e++) {
        int from, to;
        cin >> from >> to;
        vector<double> weights(N), probabilities(N);
        for (int i = 0; i < N; i++) {
            cin >> weights[i] >> probabilities[i];
        }
        G[from][to] = Edge(weights, probabilities);
    }
    int s, t;
    cin >> s >> t;
    auto ans = solve(G, s, t, number_of_DijkstraMC, number_of_PathSampling,
                     number_of_GraphSampling);
    for (auto ans : ans) {
        ans.second.print();
        cout << ans.first << endl;
    }
    return 0;
}