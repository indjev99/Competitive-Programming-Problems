#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <array>
#include <iomanip>
#include <queue>
#include <stack>
#include <numeric>
#include <cassert>
#include <cmath>
#include <random>
#include <ctime>
#include <chrono>

using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)x.size())
#define rep(i, a, b) for(int i = a; i < b; ++i)

template <typename T>
void remove_duplicates(vector<T> &v){
    sort(all(v));
    v.resize(unique(all(v)) - v.begin());
}

template <typename T> void chmin(T &a, T b){ a = (b < a) ? b : a; }
template <typename T> void chmax(T &a, T b){ a = (b > a) ? b : a; } 

vector<bool> compare(int i, int j);

vector<vector<int>> order;
int n, k;
map<pii, vector<bool>> mem_queries;

bool check_memorized(int i, int j){
    bool invert = i > j;
    if(invert) { swap(i, j); }
    return mem_queries.count({i, j});
}

vector<bool> smart_compare(int i, int j){
    bool invert = i > j;
    if(invert) { swap(i, j); }
    
    vector<bool> ans;
    if(mem_queries.count({i, j})){
        ans = mem_queries[{i, j}];
    }
    else{
        ans = mem_queries[{i, j}] = compare(i, j);
    }

    if(invert){
        for(int idx = 0; idx < k; ++idx){
            ans[idx] = !ans[idx];
        }
    }
    return ans;
}

void manual_merge(int dim, int l, int mid, int r){
    auto &arr = order[dim];

    vector<int> new_vals;

    int ptr[2]{l, mid + 1};
    while(ptr[0] != mid + 1 && ptr[1] != r + 1){

        constexpr int ITERS = 5;
        auto [x, y] = pii{arr[ptr[0]], arr[ptr[1]]};
        bool changed = false;
        for(int i = 1; i <= ITERS &&  ptr[1] + i <= r; ++i){
            if(check_memorized(x, arr[ptr[1] + i])){
                if(!smart_compare(x, arr[ptr[1] + i])[dim]){
                    changed = true;
                    ++i;
                    while(i--){
                        new_vals.push_back(arr[ptr[1]++]);
                    }
                }
                break;
            }
        }
        if(changed) { continue; }

        if(smart_compare(arr[ptr[0]], arr[ptr[1]])[dim]){
            new_vals.push_back(arr[ptr[0]++]);
        }
        else{
            new_vals.push_back(arr[ptr[1]++]);
        }
    }
    while(ptr[0] != mid + 1){
        new_vals.push_back(arr[ptr[0]++]);
    }
    while(ptr[1] != r + 1){
        new_vals.push_back(arr[ptr[1]++]);
    }

    std::copy(all(new_vals), order[dim].begin() + l);
}

void merge_sort(int l, int r){
    if(l == r) return;

    int mid = (l + r) >> 1;
    merge_sort(l, mid);
    merge_sort(mid + 1, r);

    for(int i = 0; i < k; ++i){
        vector<int> new_vals(r - l + 1);
        manual_merge(i, l, mid, r);
        // std::merge(order[i].begin() + l, order[i].begin() + mid + 1, order[i].begin() + mid + 1, order[i].begin() + r + 1, new_vals.begin(), [&](int l_val, int r_val){
            // return (bool)smart_compare(l_val, r_val)[i];
        // });
        // std::copy(all(new_vals), order[i].begin() + l);
    }
}

vector<vector<int>> pointSort(int _n, int _k){
    n = _n;
    k = _k;
    mem_queries.clear();

    order.resize(k);
    for(int i = 0; i < k; ++i){
        order[i].resize(n);
        iota(all(order[i]), 0);
    }

    merge_sort(0, n - 1);

    vector<vector<int>> ans(n, vector<int>(k));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < k; ++j){
            ans[order[j][i]][j] = i;
        }
    }
    return ans;
}

