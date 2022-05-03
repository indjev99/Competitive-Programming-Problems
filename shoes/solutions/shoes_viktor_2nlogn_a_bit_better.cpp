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

vector<pair<int, int>> guessPairs(int n){
    vector<int> curr(2 * n);
    iota(all(curr), 1);

    vector<pair<int, int>> ans;

    while(!curr.empty()){
        int l = 1, r = (int)curr.size() / 2;
        while(l != r){
            int mid = (l + r) >> 1;
            if(pairInSet(vFromTo(curr, 0, mid)))
                r = mid;
            else
                l = mid + 1;
        }

        int l2 = 0, r2 = l - 1;
        while(l2 != r2){
            int mid = (l2 + r2 + 1) >> 1;
            if(pairInSet(vFromTo(curr, mid, l)))
                l2 = mid;
            else
                r2 = mid - 1;
        }

        ans.push_back({curr[l], curr[l2]});
        curr.erase(curr.begin() + l);
        curr.erase(curr.begin() + l2);
    }

    return ans;
}