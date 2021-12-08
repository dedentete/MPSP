#include <algorithm>
#include <cfloat>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;

struct Edge {
    int from, to;
    vector<pair<double, double>> weights;

    Edge(int from, int to, vector<pair<double, double>> weights)
        : from(from), to(to), weights(weights) {}
};

struct Path {
    vector<int> vertexes;

    void reverse() {
        std::reverse(vertexes.begin(), vertexes.end());
    }

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
};

bool operator<(const Path &lhs, const Path &rhs) {
    return lhs.vertexes < rhs.vertexes;
}

Path Dijkstra(vector<vector<pair<int, double>>> &g, int &s, int &t) {
    using P = pair<double, int>;
    int V = g.size();
    vector<double> dist(V, DBL_MAX);
    priority_queue<P, vector<P>, greater<P>> que;
    dist[s] = 0;
    que.emplace(0, s);
    vector<int> prev(V, -1);
    while (!que.empty()) {
        P p = que.top();
        que.pop();
        int v = p.second;
        if (dist[v] < p.first) continue;
        for (int i = 0; i < (int)g[v].size(); i++) {
            int to = g[v][i].first;
            double cost = g[v][i].second;
            if (dist[to] > dist[v] + cost) {
                dist[to] = dist[v] + cost;
                prev[to] = v;
                que.emplace(dist[to], to);
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

void dfs(int depth, double probability, vector<Edge> &edges, int &s, int &t,
         vector<vector<pair<int, double>>> &g, map<Path, double> &cnt) {
    int E = edges.size(), N = edges[depth].weights.size();
    if (depth == E) {
        cnt[Dijkstra(g, s, t)] += probability;
        return;
    }
    for (int i = 0; i < N; i++) {
        g[edges[depth].from].emplace_back(edges[depth].to,
                                          edges[depth].weights[i].first);
        dfs(depth + 1, probability * edges[depth].weights[i].second, edges, s,
            t, g, cnt);
        g[edges[depth].from].pop_back();
    }
}

map<Path, double> solve(vector<Edge> &edges, int &s, int &t, int &V) {
    vector<vector<pair<int, double>>> g;
    g.resize(V);
    map<Path, double> cnt;
    dfs(0, 1, edges, s, t, g, cnt);
    return cnt;
}

int main() {
    int V, E, N;
    cin >> V >> E >> N;
    vector<Edge> edges;
    for (int e = 0; e < E; e++) {
        int from, to;
        cin >> from >> to;
        vector<pair<double, double>> weights(N);
        for (int i = 0; i < N; i++) {
            cin >> weights[i].first >> weights[i].second;
        }
        edges.emplace_back(from, to, weights);
    }
    int s, t;
    cin >> s >> t;
    auto cnt = solve(edges, s, t, V);
    for (auto p : cnt) {
        cout << "probability : " << p.second << endl;
        cout << "path        : ";
        p.first.print();
    }
    return 0;
}