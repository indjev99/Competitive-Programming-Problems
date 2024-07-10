#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>

using namespace std;

const int M = 50;
const int N = 10;
const int K = 1e5 + 3;

mt19937 mt(3);

int n, m, k;
double p, x, y;
int cnt[K][N];

bool is_fair[K];

double prob_stats[2][M][M][M];

array<int, 3> get_stats(int *cnt_on_side){
    int mn = m;
    int mx = 0;

    double avg = m / n;
    double dev = 0;
    for(int j = 1; j <= n; ++j){
        mx = max(mx, cnt_on_side[j]);
        mn = min(mn, cnt_on_side[j]);

        double diff = (cnt_on_side[j] - avg);
        dev += diff * diff;
    }
    dev /= n;
    dev = sqrt(dev);
    dev *= 8;
    dev = min(dev, (double)m);
    return array{mn, mx, (int)(dev + 0.5)};
}

void init(){
    int count_iters = 3e5;

    for(int i = 1; i <= count_iters; ++i){
        int cnt[N]{};
        for(int j = 1; j <= m; ++j){
            cnt[mt() % n + 1]++;
        }

        auto stats = get_stats(cnt);
        ++prob_stats[0][stats[0]][stats[1]][stats[2]];
    }

    for(int i = 1; i <= count_iters; ++i){
        int q[N]{};
        int cnt[N]{};
        for(int j = 1; j <= n; ++j){
            q[j] = mt() % (int)2e7 + 1;
            q[j] += q[j - 1];
        }

        for(int j = 1; j <= m; ++j){
            int rand_num = mt() % q[n] + 1;

            int rolled = lower_bound(q + 1, q + 1 + n, rand_num) - q;
            ++cnt[rolled];
        }

        auto stats = get_stats(cnt);
        ++prob_stats[1][stats[0]][stats[1]][stats[2]];
    }

    for(int type = 0; type <= 1; ++type){
        for(int s0 = 0; s0 <= m; ++s0){
            for(int s1 = 0; s1 <= m; ++s1){
                for(int s2 = 0; s2 <= m; ++s2){
                    prob_stats[type][s0][s1][s2] /= count_iters;
                }
            }
        }
    }
}

void solve(int idx){
    auto stats = get_stats(cnt[idx]);
    double fair_p = prob_stats[0][stats[0]][stats[1]][stats[2]] * p;
    double unfair_p = prob_stats[1][stats[0]][stats[1]][stats[2]] * (1 - p);

    if(fair_p == unfair_p && fair_p == 0){
        fair_p = unfair_p = 0.5;
    }
    else{
        double sum = fair_p + unfair_p;
        fair_p /= sum;
        unfair_p /= sum;
    }

    double fair_cost = unfair_p * y;
    double unfair_cost = fair_p * x;

    cout << (fair_cost < unfair_cost) << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m >> p >> x >> y;
    cin >> k;

    init();

    for(int i = 1; i <= k; ++i){
        for(int j = 1; j <= n; ++j){
            cin >> cnt[i][j];
        }
        solve(i);
    }
}