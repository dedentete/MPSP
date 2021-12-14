#include <algorithm>
#include <cfloat>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

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
};

Path Dijkstra(vector<vector<pair<int, double>>> &g, int s, int t) {
    using P = pair<double, int>;
    int V = g.size();
    vector<double> weight(V, DBL_MAX);
    priority_queue<P, vector<P>, greater<P>> que;
    weight[s] = 0;
    que.emplace(0, s);
    vector<int> prev(V, -1);
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

int main() {
    int V, E, N;
    cin >> V >> E >> N;
    vector<vector<pair<int, double>>> g(V);
    for (int e = 0; e < E; e++) {
        int from, to;
        cin >> from >> to;
        double sum = 0;
        for (int i = 0; i < N; i++) {
            double weight, probability;
            cin >> weight >> probability;
            sum += weight * probability;
        }
        g[from].emplace_back(to, sum);
    }
    int s, t;
    cin >> s >> t;
    auto path = Dijkstra(g, s, t);
    path.print();
    return 0;
}