#include <iostream>
#include <vector>
using namespace std;

int main() {
    int V, E, N;
    cin >> V >> E >> N;
    cout << V + E * N << " " << 2 * E * N << endl;
    for (int e = 0; e < E; e++) {
        int u, v;
        cin >> u >> v;
        vector<pair<double, double>> wp(N);
        for (int i = 0; i < N; i++) {
            cin >> wp[i].first >> wp[i].second;
        }
        double sum = 0;
        for (int i = 0; i < N; i++) {
            cout << u << " " << V + e * N + i << " " << 0 << " "
                 << wp[i].second / (1 - sum) << endl;
            cout << V + e * N + i << " " << v << " " << wp[i].first << " " << 1
                 << endl;
            sum += wp[i].second;
        }
    }
    return 0;
}