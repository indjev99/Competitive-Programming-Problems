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
    for(int i = 1; i < curr.size(); ++i){
        if(pairInSet(vFromTo(curr, 0, i))){
            int l = 0, r = i - 1;
            while(l != r){
                int mid = (l + r + 1) >> 1;
                if(pairInSet(vFromTo(curr, mid, i)))
                    l = mid;
                else
                    r = mid - 1;
            }

            ans.push_back({curr[l], curr[i]});
            curr.erase(curr.begin() + i);
            curr.erase(curr.begin() + l);

            i -= 2;
            continue;
        }
    }

    return ans;
}