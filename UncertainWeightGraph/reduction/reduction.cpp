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
        vector<double> weights(N), probabilities(N);
        for (int i = 0; i < N; i++) {
            cin >> weights[i] >> probabilities[i];
        }
        double sum = 0;
        for (int i = 0; i < N; i++) {
            cout << u << " " << V + e * N + i << " " << 0 << " "
                 << probabilities[i] / (1 - sum) << endl;
            cout << V + e * N + i << " " << v << " " << weights[i] << " " << 1
                 << endl;
            sum += probabilities[i];
        }
    }
    int s, t;
    cin >> s >> t;
    cout << s << " " << t << endl;
    cout << V << endl;
    return 0;
}