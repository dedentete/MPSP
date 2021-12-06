#include <algorithm>
#include <cfloat>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

struct Edge {
    int from, to;
    double weight, probability;

    Edge(int from, int to, double weight, double probability)
        : from(from), to(to), weight(weight), probability(probability) {}
};

bool operator==(const Edge& lhs, const Edge& rhs) {
    return lhs.from == rhs.from && lhs.to == rhs.to &&
           lhs.weight == rhs.weight && lhs.probability == rhs.probability;
}

struct Path {
    vector<Edge> edges;
    double weight, probability;

    Path() : weight(DBL_MAX), probability(1) {}

    void print() {
        for (int i = 0; i < (int)edges.size(); i++) {
            if (!i) cout << edges[i].from;
            cout << " -> " << edges[i].to;
        }
        cout << endl;
    }

    Path operator+=(Edge e) {
        if (edges.empty()) weight = 0;
        edges.emplace_back(e);
        weight += e.weight;
        probability *= e.probability;
        return *this;
    }

    Path operator+(Edge e) {
        Path res = *this;
        res += e;
        return res;
    }

    Path operator-(Path p) {
        Path res;
        for (auto edge : edges) {
            if (find(p.edges.begin(), p.edges.end(), edge) != p.edges.end())
                continue;
            res += edge;
        }
        return res;
    }
};

bool operator<(const Path& lhs, const Path& rhs) {
    return lhs.weight < rhs.weight;
}

bool WithProbability(double probability) {
    return probability >= (rand() % (int)1e9) / 1e9;
};

Path Algorithm2(vector<vector<Edge>>& G, int s, int t) {
    int V = G.size();
    int u = s;
    set<int> vis;
    vis.emplace(s);
    vector<Path> P(V);
    P[u].weight = 0;
    do {
        for (auto edge : G[u]) {
            int v = edge.to;
            if (vis.count(v)) continue;
            if (P[v].weight > P[u].weight + edge.weight) {
                if (WithProbability(edge.probability)) {
                    P[v] = P[u] + edge;
                }
            }
        }
        double mn = DBL_MAX;
        for (int v = 0; v < V; v++) {
            if (vis.count(v)) continue;
            if (P[v].weight < mn) {
                u = v;
                mn = P[v].weight;
            }
        }
        if (mn == DBL_MAX) break;
        vis.emplace(u);
    } while (u != t);
    return P[t];
}

double Algorithm3(vector<vector<Edge>>& G, int s, int t, Path P,
                  vector<Path> SP, int N) {
    if (SP.empty()) {
        return P.probability;
    }
    int C = 0;
    double S = 0;
    for (auto SP : SP) {
        S += (SP - P).probability;
    }
    auto SamplePath = [&]() {
        double t = S * (rand() % (int)1e9) / 1e9;
        for (int i = 0; i < (int)SP.size(); i++) {
            t -= (SP[i] - P).probability;
            if (t <= 0) return i;
        }
        return -1;
    };
    auto SamplePossibleWorld = [](vector<vector<Edge>>& G, Path A) {
        int V = G.size();
        vector<vector<Edge>> res(V);
        for (int v = 0; v < V; v++) {
            for (auto edge : G[v]) {
                if (find(A.edges.begin(), A.edges.end(), edge) !=
                        A.edges.end() ||
                    WithProbability(edge.probability)) {
                    res[v].emplace_back(edge);
                }
            }
        }
        return res;
    };
    auto InPossibleWorld = [](vector<vector<Edge>>& g, Path A) {
        int cnt = 0;
        int V = g.size();
        for (int v = 0; v < V; v++) {
            for (auto edge : g[v]) {
                if (find(A.edges.begin(), A.edges.end(), edge) !=
                    A.edges.end()) {
                    cnt++;
                }
            }
        }
        return cnt == (int)A.edges.size();
    };
    for (int r = 0; r < N; r++) {
        int i = SamplePath();
        auto g = SamplePossibleWorld(G, SP[i] - P);
        bool flag = false;
        for (int j = 0; j < i; j++) {
            if (InPossibleWorld(g, SP[j] - P)) {
                flag = true;
                break;
            }
        }
        if (!flag) C++;
    }
    double p = (double)C / N * S;
    return (1 - p) * P.probability;
}

vector<pair<double, Path>> Algorithm1(vector<vector<Edge>>& G, int s, int t,
                                      int m, int N) {
    set<Path> CP;
    for (int i = 0; i < m; i++) {
        auto P = Algorithm2(G, s, t);
        if (!P.edges.empty()) {
            CP.emplace(P);
        }
    }
    if (CP.empty()) return vector<pair<double, Path>>();
    cout << "size        : " << CP.size() << endl;
    vector<Path> LP;
    for (auto P : CP) {
        LP.emplace_back(P);
    }
    vector<pair<double, Path>> res;
    for (int i = 0; i < (int)LP.size(); i++) {
        vector<Path> SP(LP.begin(), LP.begin() + i);
        res.emplace_back(Algorithm3(G, s, t, LP[i], SP, N), LP[i]);
    }
    sort(res.begin(), res.end());
    reverse(res.begin(), res.end());
    return res;
}

int main(int argc, char const* argv[]) {
    int m = atoi(argv[1]), N = atoi(argv[2]);
    int V, E;
    cin >> V >> E;
    vector<vector<Edge>> G(V);
    for (int e = 0; e < E; e++) {
        int from, to;
        double weight, probability;
        cin >> from >> to >> weight >> probability;
        G[from].emplace_back(from, to, weight, probability);
    }
    int s, t;
    cin >> s >> t;
    auto ans = Algorithm1(G, s, t, m, N);
    for (auto ans : ans) {
        cout << "probability : " << ans.first << endl;
        cout << "path        : ";
        ans.second.print();
    }
    return 0;
}