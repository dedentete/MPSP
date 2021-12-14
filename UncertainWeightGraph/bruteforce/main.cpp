#include <algorithm>
#include <cfloat>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;

struct Edge {
    int from, to;
    vector<double> weights, probabilities;

    Edge(int from, int to, vector<double> weights, vector<double> probabilities)
        : from(from), to(to), weights(weights), probabilities(probabilities) {}
};

struct Path {
    vector<int> vertexes;

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

    bool operator<(const Path &p) const {
        return vertexes < p.vertexes;
    }
};

Path Dijkstra(vector<vector<pair<int, double>>> &g, int s, int t) {
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
        for (int i = 0; i < (int)g[v].size(); i++) {
            int to = g[v][i].first;
            double cost = g[v][i].second;
            if (weight[to] > weight[v] + cost) {
                weight[to] = weight[v] + cost;
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

void dfs(int depth, double probability, vector<Edge> &edges, int s, int t,
         vector<vector<pair<int, double>>> &g, map<Path, double> &cnt) {
    int E = edges.size(), N = edges[depth].weights.size();
    if (depth == E) {
        cnt[Dijkstra(g, s, t)] += probability;
        return;
    }
    for (int i = 0; i < N; i++) {
        g[edges[depth].from].emplace_back(edges[depth].to,
                                          edges[depth].weights[i]);
        dfs(depth + 1, probability * edges[depth].probabilities[i], edges, s,
            t, g, cnt);
        g[edges[depth].from].pop_back();
    }
}

vector<pair<double, Path>> solve(vector<Edge> &edges, int s, int t, int V) {
    vector<vector<pair<int, double>>> g;
    g.resize(V);
    map<Path, double> cnt;
    dfs(0, 1, edges, s, t, g, cnt);
    vector<pair<double, Path>> res;
    for (auto p : cnt) {
        res.emplace_back(p.second, p.first);
    }
    sort(res.begin(), res.end());
    reverse(res.begin(), res.end());
    return res;
}

int main() {
    int V, E, N;
    cin >> V >> E >> N;
    vector<Edge> edges;
    for (int e = 0; e < E; e++) {
        int from, to;
        cin >> from >> to;
        vector<double> weights(N), probabilities(N);
        for (int i = 0; i < N; i++) {
            cin >> weights[i] >> probabilities[i];
        }
        edges.emplace_back(from, to, weights, probabilities);
    }
    int s, t;
    cin >> s >> t;
    auto ans = solve(edges, s, t, V);
    for (auto ans : ans) {
        ans.second.print();
        cout << ans.first << endl;
    }
    return 0;
}