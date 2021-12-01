#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

int main(int argc, char const* argv[]) {
    int V = atoi(argv[1]), E = atoi(argv[2]), N = atoi(argv[3]);
    cout << V << " " << E << " " << N << endl;
    set<pair<int, int>> edged;
    for (int e = 0; e < E; e++) {
        int u = rand() % V, v = rand() % V;
        while (u == v || edged.count(pair<int, int>(u, v))) {
            u = rand() % V;
            v = rand() % V;
        }
        edged.emplace(pair<int, int>(u, v));
        vector<double> p;
        p.emplace_back(0);
        p.emplace_back(1);
        for (int i = 0; i < N - 1; i++) {
            p.emplace_back((rand() % (int)1e9) / 1e9);
        }
        sort(p.begin(), p.end());
        vector<double> w(N);
        for (int i = 0; i < N; i++) {
            w[i] = (rand() % (int)1e9) / 1e9;
        }
        sort(w.begin(), w.end());
        cout << u << " " << v << endl;
        for (int i = 0; i < N; i++) {
            cout << w[i] << " " << p[i + 1] - p[i] << endl;
        }
    }
    return 0;
}