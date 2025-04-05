#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <set>
#include <iostream>

using namespace std;

const int BITS = 14;

void add_number(vector<bool> &output, int num){
    for(int i = 0; i < BITS; ++i){
        output.push_back((num >> i) & 1);
    }
}

std::vector<bool> encode(int n, std::vector<std::pair<int, int>> edges){
    vector<bool> output;
    vector<vector<int>> adj(n);
    for(auto [a, b]: edges){
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<bool> vis(n);
    vector<int> par(n), depth(n);

    function<void(int)> dfs = [&](int u){
        vis[u] = true;
        for(int to: adj[u]){
            if(vis[to]) { continue; }
            par[to] = u;
            depth[to] = depth[u] + 1;
            dfs(to);
        }
    };

    for(int i = 0; i < n; ++i){
        if(!vis[i]){
            par[i] = -1;
            dfs(i);
        }
    }

    for(int i = 0; i < n; ++i){
        add_number(output, par[i] + 1);
    }

    set<pair<int, int>> rem_edges;
    for(auto [a, b]: edges){
        if(a > b){
            swap(a, b);
        }
        rem_edges.insert({a, b});
    }

    for(int node = 0; node < n; ++node){
        if(par[node] != -1){
            int a = node;
            int b = par[node];
            if(a > b) { swap(a, b); }

            rem_edges.erase({a, b});
        }
    }

    vector<bool> up2(n);
    for(auto [a, b]: rem_edges){
        if(depth[a] > depth[b]){
            swap(a, b);
        }

        int len = depth[b] - depth[a];
        for(int i = 0; i < len - 1; ++i){
            up2[b] = true;
            b = par[b];
        }
    }

    for(int i = 0; i < n; ++i){
        output.push_back(up2[i]);
    }

    return output;
}


int decode_number(vector<bool> &input, int start){
    int num = 0;
    for(int i = 0; i < BITS; ++i){
        num += ((int)input[start + i]) << i;
    }
    return num;
}

std::vector<std::pair<int, int>> decode(int n, std::vector<bool> data){
    vector<pair<int, int>> ans;
    vector<vector<int>> adj(n);
    vector<int> p(n);
    int data_ptr = 0;
    for(int i = 0; i < n; ++i, data_ptr += BITS){
        int par = decode_number(data, data_ptr) - 1;
        if(par != -1){
            ans.push_back({i, par});
            adj[par].push_back(i);
        }
        p[i] = par;
    }

    vector<bool> up2(n);
    for(int i = 0; i < n; ++i){
        up2[i] = data[data_ptr + i];
    }

    vector<int> depth(n);
    function<void(int)> dfs = [&](int u){
        for(int to: adj[u]){
            depth[to] = depth[u] + 1;
            dfs(to);
        }
    };

    for(int i = 0; i < n; ++i){
        if(p[i] == -1){
            dfs(i);
        }
    }

    vector<pair<int, int>> order;
    for(int i = 0; i < n; ++i){
        order.push_back({depth[i], i});
    }
    sort(order.begin(), order.end());

    vector<set<int>> conn(n);
    vector<int> curr(n, -1);
    for(auto [du, u]: order){
        if(!up2[u]) { continue; }
        
        if(!up2[p[u]]){
            conn[p[p[u]]].insert(u);
            curr[u] = p[p[u]];
        }
        else{
            curr[u] = curr[p[u]];
            conn[curr[u]].erase(p[u]);
            conn[curr[u]].insert(u);
        }
    }

    for(int i = 0; i < n; ++i){
        for(int x: conn[i]){
            ans.push_back({i, x});
        }
    }

    return ans;
}
/*
1
7 8
0 1
1 2
2 3
3 0
0 4
4 5
5 6
6 0
*/