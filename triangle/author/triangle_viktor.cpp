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

template<typename C> struct rge{C l, r;};
template<typename C> rge<C> range(C i, C j) { return rge<C>{i, j}; }
template<typename C> ostream& operator<<(ostream &os, rge<C> r) { os << '{'; for(auto it = r.l; it != r.r; it++) os << "," + (it == r.l) << *it; os << '}'; return os; }
template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '{' << p.first << "," << p.second << '}'; }
template<typename T_container, typename T = typename enable_if<!is_same<T_container, string>::value, typename T_container::value_type>::type> ostream& operator<<(ostream &os, const T_container &v) { os << '{'; string sep; for (const T &x : v) os << sep << x, sep = ","; return os << '}'; }
void dbg_out() { cerr << ']' << endl; }
template<typename A> void dbg_out(A H) { cerr << H; dbg_out(); }
template<typename A, typename B, typename... C> void dbg_out(A H, B G, C... T) { cerr << H << ","; dbg_out(G, T...); }
#ifdef DEBUG
#define debug(...) cerr << "[" << #__VA_ARGS__ << "] = [", dbg_out(__VA_ARGS__)
#else
#define debug(...)
#endif

template <typename T>
void remove_duplicates(vector<T> &v){
    sort(all(v));
    v.resize(unique(all(v)) - v.begin());
}

template <typename T> void chmin(T &a, T b){ a = (b < a) ? b : a; }
template <typename T> void chmax(T &a, T b){ a = (b > a) ? b : a; } 

#include "triangle.h"

map<array<int, 3>, bool> mem_res;
bool mem_query(int a, int b, int c) {
    array<int, 3> arr{a, b, c};
    sort(all(arr));

    auto it = mem_res.find(arr);
    if (it != mem_res.end()) {
        return it->second;
    }
    return mem_res[arr] = query(a - 1, b - 1, c - 1);
}

bool comp(int a, int b) {
    return mem_query(a, a, b);
}

int get_num(int idx, const vi &v) {
    int cnt = 0;
    for (int j: v) {
        if (j == idx) { ++cnt; }
        else {
            cnt += comp(idx, j);
        }
    }
    
    if (cnt != v.size()) {
        return cnt / 2 + 1;
    }
    return -1;
}

std::vector<int> brute(vector<int> v) {
    int n = v.size();
    int cnt_queries = 0;

    vector<int> known(n + 1);
    for (int i = 0; i < n; ++i) {
        int num = get_num(v[i], v);
        if (num != -1) {
            known[num] = v[i];
        }
    }

    vector<int> leftover = v;
    for (int i = 1; i <= n / 2; ++i) {
        int x = known[i];
        leftover.erase(find(all(leftover), x));
    }

    for (int i = n / 2 + 1; i < n; ++i) {
        int a = known[n / 2];
        int b = known[i - n / 2];
        int c = known[i - n / 2 + 1];

        for (int x: leftover) {
            if (!mem_query(a, b, x) && mem_query(a, c, x)) {
                known[i] = x;
                // cerr << "known " << i << " " << x << endl;
            }
        }
    }

    for (int x: known) {
        auto iter = find(all(leftover), x);
        if (iter != leftover.end()) {
            leftover.erase(iter);
        }
    }


    for (int i = n - leftover.size() + 1; i <= n; ++i) {
        int a = known[n - 2];
        int b = known[i - (n - 2)];
        int c = known[i - (n - 2) + 1];
        for (int x: leftover) {
            if (!mem_query(a, b, x) && mem_query(a, c, x)) {
                known[i] = x;
                // cerr << "known " << i << " " << x << endl;
            }
        }
    }

    return known;
}

pair<vi, vi> separate(int idx, const vi &v, int idx2 = -1, bool fast=false) { 
    if (idx2 == -1) idx2 = idx;

    vi l, r;
    for (int j: v) {
        if (j == idx) { l.push_back(idx); }
        else {
            if (mem_query(idx, idx2, j)) l.push_back(j);
            else r.push_back(j);
        }
        if (fast && l.size() >= 8 && r.size() == 0) {
            return {v, {}};
        }
    }
    return {l, r};
}

void dnc(int l, int r, const vi &known, vi indices, vi &new_known) {
    // cerr << "dnc " << l << " " << r << endl;
    if (l == r) {
        new_known[l] = indices[0];
        return;
    }

    int mid = (l + r) >> 1;

    auto [lidx, ridx] = separate(known[mid / 2 + 1], indices, known[(mid + 1) / 2]);

    dnc(l, mid, known, lidx, new_known);
    dnc(mid + 1, r, known, ridx, new_known);
}

vi extend(vi known, vi r) {
    auto new_known = known;
    new_known.resize(known.size() + r.size());
    dnc(known.size(), known.size() + r.size() - 1, known, r, new_known);
    return new_known;
}
mt19937 mt(32);

std::vector <int> solve(int n) {
    mem_res.clear();

    vector<int> v(n);
    iota(all(v), 1);

    pii p;
    auto small_nums = v;
    while(true) {
        shuffle(all(small_nums), mt);
        int i = small_nums[0];
        // int num = get_num(i, small_nums);

        auto new_nums = separate(i, small_nums, -1, true).first;
        int num = (new_nums.size() == small_nums.size()) ? -1 : new_nums.size() / 2 + 1;

        const int C = 20;
        if (num != -1 && (num * 2 - 1) >= 4 && num < C){
            p = {num, i};
            break;
        }
        if (num != -1 && num * 2 - 1 >= 4) {
            small_nums = new_nums;
        }
    }



    auto [l, r] = separate(p.second, v);  
    auto known =  brute(l);
    while(known.size() != n + 1) {
        // cerr << separate(known.back(), r).first.size() << " " << separate(known.back(), r).second.size() << endl; 
        known = extend(known, separate(known.back(), r).first);
        for (int x: known) {
            auto it = find(all(r), x);
            if (it != r.end()) {
                r.erase(it);
            }
        }
        // cerr << known.size() << endl;
    }

    vector<int> ans(n);
    for (int i = 1; i < known.size(); ++i) {
        if (known[i])
        ans[known[i] - 1] = i; 
    }
    return ans;
}