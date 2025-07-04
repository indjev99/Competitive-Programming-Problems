#include <vector>
#include <utility>
#include <functional>
#include <cassert>
#include <random>

using namespace std;

#define all(x) (x).begin(), (x).end()

const int LEN_BITSUM = 8;

std::vector<std::pair<int, int>> encode(int n, std::vector<bool> data) {
    mt19937 mt(3);

    std::vector<std::pair<int, int>> edges;

    for (int i = 0; i < data.size(); ++i) {
        data[i] = data[i] ^ (mt() % 2);
    }

    int start = 0;
    for (int i = 1; i <= 6; ++i) {
        edges.push_back({start, i});
    }

    int end = edges.size() + 1;
    for(int i = 1; i <= 4; ++i) {
        edges.push_back({end, end + i});
    }

    int node_ptr = edges.back().second + 1;
    int len_bitsum = LEN_BITSUM;
    int len_guess = (n - node_ptr) * 100 / 170  - len_bitsum;

    vector<bool> data_with_bitsum;
    for (int i = 0; i < len_guess; ++i) {
        data_with_bitsum.push_back(data[i]);
    }

    for (int bit = 0; bit < len_bitsum - 1; ++bit) {
        int xor_bit = 0;
        for (int i = 0; i < len_guess; ++i){
            if ((i + 1) >> bit & 1) {
                xor_bit ^= data[i];
            }
        }
        data_with_bitsum.push_back(xor_bit);
    }

    {
        int total_xor = 0;
        for (int i = 0; i < len_guess; ++i) {
            total_xor ^= data[i];
        }
        data_with_bitsum.push_back(total_xor);
    }

    int prev_node = start;
    for (int i = 0; i < data_with_bitsum.size(); ++i) {
        edges.push_back({node_ptr, prev_node});
        prev_node = node_ptr;

        if (data_with_bitsum[i]) {
            edges.push_back({node_ptr, node_ptr + 1});
            node_ptr += 2;
        }
        else {
            node_ptr += 1;
        }
    }
    edges.push_back({prev_node, end});

    for(; node_ptr < n; ++node_ptr) {
        edges.push_back({0, node_ptr});
    }

    return edges;
}

vector<int> get_path(vector<vector<int>> adj, int start, int end){
    vector<bool> vis(adj.size());
    vector<int> path;

    function<bool(int)> dfs = [&](int u) {
        vis[u] = true;

        if (u == end) {
            path.push_back(end);
            return true;
        }

        for (int to: adj[u]) {
            if (vis[to]) {
                continue;
            }

            if (dfs(to)) {
                path.push_back(u);
                return true;
            }
        }

        return false;
    };

    dfs(start);
    reverse(all(path));

    return path;
}

vector<bool> correct(vector<bool> data) {
    vector<bool> corrected;
    for (int i = 0; i < (int)data.size() - LEN_BITSUM; ++i) {
        corrected.push_back(data[i]);
    }

    int wrong = 0;
    for (int bit = 0; bit < LEN_BITSUM - 1; ++bit) {
        int xor_bit = 0;
        for (int i = 0; i < (int)data.size() - LEN_BITSUM; ++i) {
            if((i + 1) >> bit & 1) {
                xor_bit ^= data[i];
            }
        }
        if (xor_bit != data[(int)data.size() - LEN_BITSUM + bit]) {
            wrong += 1 << bit;
        }
    }

    --wrong;

    int total_xor = 0;
    for (int bit: corrected) {
        total_xor ^= bit;
    }

    if (total_xor != data.back()) {
        if (wrong == -1) {
            return corrected;
        }
        corrected[wrong] = !corrected[wrong];
        return corrected;
    }
    return corrected;
}

std::vector<bool> decode(int n, std::vector<std::pair<int, int>> tree) {
    std::vector<std::vector<int>> adj(n + 1);
    for(const auto& [u, v]: tree) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int start = -1, end = -1;
    for (int i = 0; i < n + 1; ++i) {
        if(adj[i].size() >= 7) {
            start = i;
        }
        else if(adj[i].size() >= 5) {
            end = i;
        }
    }

    while(start == -1 || end == -1);

    vector<int> path = get_path(adj, start, end);

    vector<bool> data;
    for (int i = 1; i < (int)path.size() - 1; ++i) {
        data.push_back(adj[path[i]].size() >= 3);
    }

    data = correct(data);

    mt19937 mt(3);
    for (int i = 0; i < data.size(); ++i) {
        data[i] = data[i] ^ (mt() % 2);
    }
    return data;
}