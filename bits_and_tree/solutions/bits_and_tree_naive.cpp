#include <vector>
#include <utility>

std::vector<std::pair<int, int>> encode(int n, std::vector<bool> data) {
    std::vector<std::pair<int, int>> edges;
    if (data[0]) {
        for (int i = 1; i < n; ++i) {
            edges.push_back({0, i});
        }
    }
    else{
        for (int i = 1; i < n; ++i) {
            edges.push_back({i - 1, i});
        }
    }
    return edges;
}

std::vector<bool> decode(int n, std::vector<std::pair<int, int>> tree) {
    std::vector<std::vector<int>> adj(n + 1);
    for(const auto& [u, v]: tree) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 0; i < n + 1; ++i) {
        if (adj[i].size() >= n - 1) {
            return {true};
        }
    }
    return {false};
}