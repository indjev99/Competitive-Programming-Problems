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
    max_time = 0;
    cnt.clear();
    curr.clear();

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

    ll curr_sum = 0;
    int leftover = 0;
    ll ans = 1e30;

    int ptr = -1;
    for(int t = 0; curr[0] != n; ++t){
        leftover += cnt[t];

        if(leftover >= m){
            for(int i = 0; i < m; ++i){
                ++curr[i];
                curr_sum += costs[i];
            }
            ptr = m - 1;
        }
        else{
            for(int i = 0; i < leftover; ++i){
                ++curr[i];
                curr_sum += costs[i];
            }
            ptr = max(ptr, leftover - 1);

            for(int i = leftover; i < m; ++i){
                while(ptr > i && curr[ptr] == 0)
                    --ptr;
                if(ptr <= i) break;
                curr_sum += costs[i];
                curr_sum -= costs[ptr];
                ++curr[i];
                --curr[ptr];
            }
        }
        leftover = max(0, leftover - m);

        if(t >= max_time && leftover == 0){
            ll cand = curr_sum;
            ans = min(cand * (t + 1), ans);
        }
    }
    return ans;
}
