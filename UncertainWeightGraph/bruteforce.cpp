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

vector<int> Dijkstra(int &s, int &t, vector<vector<pair<int, double>>> &G) {
    using P = pair<double, int>;
    int V = G.size();
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
        for (int i = 0; i < (int)G[v].size(); i++) {
            int to = G[v][i].first;
            double cost = G[v][i].second;
            if (dist[to] > dist[v] + cost) {
                dist[to] = dist[v] + cost;
                prev[to] = v;
                que.emplace(dist[to], to);
            }
        }
    }
    vector<int> path;
    int cur = t;
    while (cur != -1) {
        path.emplace_back(cur);
        cur = prev[cur];
    }
    reverse(path.begin(), path.end());
    return path;
}

void dfs(int depth, double probability, int &s, int &t, vector<Edge> &edges,
         vector<vector<pair<int, double>>> &g, map<vector<int>, double> &mp) {
    int E = edges.size(), N = edges[depth].weights.size();
    if (depth == E) {
        mp[Dijkstra(s, t, g)] += probability;
        return;
    }
    for (int i = 0; i < N; i++) {
        g[edges[depth].from].emplace_back(edges[depth].to,
                                          edges[depth].weights[i].first);
        dfs(depth + 1, probability * edges[depth].weights[i].second, s, t,
            edges, g, mp);
        g[edges[depth].from].pop_back();
    }
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
    vector<vector<pair<int, double>>> g;
    g.resize(V);
    map<vector<int>, double> ans;
    dfs(0, 1, s, t, edges, g, ans);
    for (auto ans : ans) {
        cout << "probability : " << ans.second << endl;
        cout << "path        : ";
        for (int i = 0; i < (int)ans.first.size(); i++) {
            if (i) cout << " -> ";
            cout << ans.first[i];
        }
        cout << endl;
    }
    return 0;
}