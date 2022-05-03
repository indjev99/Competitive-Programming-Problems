#include <bits/stdc++.h>
#include "shoes.h"

using namespace std;

typedef long long ll;
template<class T> void check_min(T &a, const T &b){ a = (a < b) ? a : b; }
template<class T> void check_max(T &a, const T &b){ a = (a > b) ? a : b; }
#define all(x) (x).begin(), (x).end()

vector<int> vFromTo(const vector<int> &v, int l, int r){
    vector<int> res;
    for(int i = l; i <= r; ++i)
        res.push_back(v[i]);
    return res;
}

vector<int> joinVectors(const vector<int> &v1, const vector<int> &v2){
    vector<int> res = v1;
    for(int x: v2)
        res.push_back(x);
    return res;
}

vector<pair<int, int>> guessPairs(int n){
    vector<int> curr(n);
    iota(all(curr), n + 1);

    vector<pair<int, int>> ans;
    for(int i = 1; i <= n; ++i){
        int l = 0, r = (int)curr.size() - 1;
        while(l != r){
            int mid = (l + r) >> 1;
            if(pairInSet(joinVectors({i}, vFromTo(curr, 0, mid))))
                r = mid;
            else
                l = mid + 1;
        }
        ans.push_back({i, curr[l]});
        curr.erase(curr.begin() + l);
    }

    return ans;
}