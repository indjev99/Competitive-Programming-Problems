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
    vector<int> v1, v2;
    v1.push_back(1);
    for(int i = 2; i <= 2 * n; ++i){
        v1.push_back(i);
        if(pairInSet(v1)){
            v1.pop_back();
            v2.push_back(i);
        }
    }

    vector<pair<int, int>> ans;
    for(int i = 0; i < n; ++i){
        int l = 0, r = (int)v2.size() - 1;
        while(l != r){
            int mid = (l + r) >> 1;
            if(pairInSet(joinVectors({v1[i]}, vFromTo(v2, 0, mid))))
                r = mid;
            else
                l = mid + 1;
        }
        ans.push_back({v1[i], v2[l]});
        v2.erase(v2.begin() + l);
    }

    return ans;
}