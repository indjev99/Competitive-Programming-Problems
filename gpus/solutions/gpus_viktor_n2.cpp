#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef __int128 ll;

int m, n;
vector<int> costs, times;
int max_time;

vector<int> cnt;
vector<int> curr;

ll solveGpus(vector<int>& _costs, vector<int>& _times)
{
    cnt.clear();
    curr.clear();
    max_time = 0;

    m = _costs.size();
    n = _times.size();
    swap(costs, _costs);
    swap(times, _times);
    
    for(int i = 0; i < n; ++i){
        max_time = max(max_time, times[i]);
    }
    cnt.resize(max_time + n + 1);
    for(int i = 0; i < n; ++i){
        ++cnt[times[i]];
    }
    curr.resize(m);

    int leftover = 0;
    ll ans = 1e30;
    for(int t = 0; curr[0] != n; ++t){
        leftover += cnt[t];

        if(leftover >= m){
            for(int i = 0; i < m; ++i){
                ++curr[i];
            }
        }
        else{
            for(int i = 0; i < leftover; ++i)
                ++curr[i];

            int ptr = m - 1;
            for(int i = leftover; i < m; ++i){
                while(ptr > i && curr[ptr] == 0)
                    --ptr;
                if(ptr <= i) break;
                ++curr[i];
                --curr[ptr];
            }
        }
        leftover = max(0, leftover - m);

        if(t >= max_time && leftover == 0){
            ll cand = 0;
            for(int i = 0; i < m; ++i){
                cand += (ll)costs[i] * curr[i];
            }
            ans = min(cand * (t + 1), ans);
        }
    }
    return ans;
}
