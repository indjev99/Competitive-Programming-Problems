#include <vector>
#include <utility>
#include <functional>
#include <algorithm>

const int BITS = 6;

using namespace std;

std::vector<std::pair<int, int>> encode(int n, std::vector<bool> data) {
    std::vector<std::pair<int, int>> edges;
    
    int mask = 0;
    for (int i = 0; i < BITS; ++i) {
        mask = 2 * mask + data[i];
    }

    int max_degree = (mask + 1) * 2;
    for (int i = 1; i <= max_degree; ++i) {
        edges.push_back({0, i});
    }
    for (int i = max_degree + 1; i < n; ++i) {
        edges.push_back({i - 1, i});
    }
    return edges;
}

std::vector<bool> decode(int n, std::vector<std::pair<int, int>> tree) {
    std::vector<std::vector<int>> adj(n + 1);
    for(const auto& [u, v]: tree) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int max_degree = max_element(adj.begin(), adj.end(), [&](auto l, auto r){
        return l.size() < r.size();
    })->size();

    int mask = max_degree / 2 - 1;
    vector<bool> data(BITS);
    for (int i = BITS - 1; i >= 0; --i) {
        data[i] = mask & 1;
        mask /= 2;
    }
    return data;
}