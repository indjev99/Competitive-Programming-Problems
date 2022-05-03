#include <bits/stdc++.h>
#include "shoes.h"

using namespace std;

typedef long long ll;
template<class T> void check_min(T &a, const T &b){ a = (a < b) ? a : b; }
template<class T> void check_max(T &a, const T &b){ a = (a > b) ? a : b; }
#define all(x) (x).begin(), (x).end()

vector<pair<int, int>> guessPairs(int n){
    vector<pair<int, int>> ans;
    vector<int> curr(2 * n);
    iota(all(curr), 1);

    while(!curr.empty()){
        for(int i = 1; i < curr.size(); ++i){
            if(pairInSet({curr[0], curr[i]})){
                ans.push_back({curr[0], curr[i]});
                curr.erase(curr.begin() + i);
                curr.erase(curr.begin());
                break;
            }
        }
    }

    return ans;
}