#include <vector>
#include <utility>
#include <functional>
#include <cassert>
#include <random>

using namespace std;

#define all(x) (x).begin(), (x).end()

const int N = 200;
const int LEN_HASH = 17;

std::vector<std::pair<int, int>> encode(int n, std::vector<bool> data) {
    std::vector<std::pair<int, int>> edges;

    int start = 0;
    for (int i = 1; i <= 6; ++i) {
        edges.push_back({start, i});
    }

    int end = edges.size() + 1;
    for(int i = 1; i <= 4; ++i) {
        edges.push_back({end, end + i});
    }

    int node_ptr = edges.back().second + 1; // 12
    int len_hash = LEN_HASH;
    int len_guess = (n - node_ptr - 2) - len_hash; // n - 14 - len_hash

    vector<bool> data_with_hash;
    for (int i = 0; i < len_guess; ++i) {
        data_with_hash.push_back(data[i]);
    }

    mt19937 mt(3);
    for(int i = 0; i < len_hash; ++i) {
        int xor_bit = 0;
        for(int j = 0; j < len_guess; ++j) {
            if (mt() % 2) {
                xor_bit ^= data[j];
            }
        }
        data_with_hash.push_back(xor_bit);
    }

    edges.push_back({start, node_ptr});

    int last_node[2]{};
    last_node[0] = last_node[1] = node_ptr;
    ++node_ptr;

    for (int i = 0; i < data_with_hash.size(); ++i) {
        if (data_with_hash[i]) {
            edges.push_back({last_node[1], node_ptr});
            last_node[1] = node_ptr;
        }
        else {
            edges.push_back({last_node[0], node_ptr});
            last_node[0] = last_node[1] = node_ptr;
        }

        ++node_ptr;
    }
    edges.push_back({last_node[0], end});

    for(; node_ptr < n; ++node_ptr) {
        edges.push_back({start, node_ptr});
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

bool verify(vector<bool> data) {
    mt19937 mt(3);
    for(int i = 0; i < LEN_HASH; ++i) {
        int xor_bit = 0;
        for(int j = 0; j < (int)data.size() - LEN_HASH; ++j) {
            if (mt() % 2) {
                xor_bit ^= data[j];
            }
        }
        if (data[(int)data.size() - LEN_HASH + i] != xor_bit) {
            return false;
        }
    }
    return true;
}

vector<bool> get_data(const vector<int> &path, const vector<vector<int>> &adj) {
    vector<bool> data;

    for (int i = 1; i < (int)path.size() - 1; ++i) {
        int x = path[i];
        int prv = path[i - 1], nxt = path[i + 1];

        function<int(int, int)> get_len = [&](int curr, int par) {
            int len = 1;
            for(int to: adj[curr]) {
                if (to == par) {
                    continue;
                }

                len = max(len, get_len(to, curr) + 1);
            }
            return len;
        };

        vector<int> lens;
        for (int to: adj[x]) {
            if (to == prv || to == nxt) {
                continue;
            }

            lens.push_back(get_len(to, x));
        }
        
        if (!lens.empty()) {
            int len = *std::max_element(lens.begin(), lens.end());
            while (len--) {
                data.push_back(true);
            }
        }
        data.push_back(false);
    }
    data.pop_back();

    return data;
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

    vector<int> path = get_path(adj, start, end);
    vector<bool> data = get_data(path, adj);

    if (verify(data)) {
        return data;
    }

    for (int i = 0; i < data.size(); ++i) {
        if (data[i]) {
            auto corrected = data;
            corrected.erase(corrected.begin() + i);
            if (verify(corrected)) {
                for (int j = 0; j <  LEN_HASH; ++j) {
                    corrected.pop_back();
                }
                return corrected;
            }
        }
    }

    while(true);
    return {};
}