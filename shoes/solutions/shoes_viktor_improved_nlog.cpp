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
    vector<int> all_shoes(2 * n + 1);
    iota(all(all_shoes), 0);

    vector<pair<int, int>> ans;
    vector<int> v1, v2;
    v1.push_back(1);

    int i = 2;
    while(i <= 2 * n){
        if(v1.size() > v2.size()){
            swap(v1, v2);
        }

        for(int j = 1; i + j - 1 <= 2 * n; j *= 2){
            vector<int> t = joinVectors(vFromTo(all_shoes, i, i + j - 1), v1);
            if(pairInSet(t)){
                for(int k = i; k <= i + (j / 2) - 1; ++k)
                    v1.push_back(k);
                i += (j / 2);
                if(j == 1 || j == 2){
                    int l = 0, r = v1.size() - 1;
                    while(l != r){
                        int mid = (l + r) >> 1;
                        if(pairInSet(joinVectors({i}, vFromTo(v1, 0, mid))))
                            r = mid;
                        else
                            l = mid + 1;
                    }
                    ans.push_back({i, v1[l]});
                    v1.erase(v1.begin() + l);
                    ++i;
                }
                break;
            }
            else if(i + 2 * j - 1 > 2 * n){
                for(int k = i; k <= i + j - 1; ++k)
                    v1.push_back(k);
                i += j;
            }
        }
    }

    while(!v1.empty()){
        int x = v1.back();
        v1.pop_back();

        int l = 0, r = v2.size() - 1;
        while(l != r){
            int mid = (l + r) >> 1;
            if(pairInSet(joinVectors({x}, vFromTo(v2, 0, mid))))
                r = mid;
            else
                l = mid + 1;
        }

        ans.push_back({x, v2[l]});
        v2.erase(v2.begin() + l);
    }

    return ans;
}