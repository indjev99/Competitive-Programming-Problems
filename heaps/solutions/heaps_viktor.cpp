#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
template<class T> void check_min(T &a, const T &b){ a = (a < b) ? a : b; }
template<class T> void check_max(T &a, const T &b){ a = (a > b) ? a : b; }
#define all(x) (x).begin(), (x).end()

const int Q = 10 + 3;
const int N = 1e4 + 3;
const int K = 3000 + 3;
const int S = 1e7 + 3;

int k, q;
pair<int, int> grundy[K][K];
int n;

void calc_grundy(){
    grundy[0][0] = {0, 0};
    for(int i = 1; i < K; ++i){
        vector<pair<int, int>> v;
        for(int j = i - 1; j >= 0; --j){
            if(i - j - 1 <= j){
                v.push_back(grundy[j][i - j - 1]);    
            }
            else{
                pair<int, int> to_push = grundy[j][j];
                to_push.second += ((i - j - 1) - j) * k;
                v.push_back(to_push);
            }
        }
        sort(all(v));
        for(int j = 0; j < i; ++j)
            grundy[i][j] = v[j];
        grundy[i][i] = {i, 0};
    }
}

pair<int, int> get_grundy(int b, int s){
    if(!k) return {b, s};
    int idx = s / k;
    pair<int, int> ret;
    if(idx <= b) ret = grundy[b][idx];
    else{
        ret = grundy[b][b];
        ret.second += (idx - b) * k;
    }
    ret.second += s % k;
    return ret;
}

void solve(){
    cin >> n;
    pair<int, int> x{0, 0};
    for(int i = 0; i < n; ++i){
        int b, s;
        cin >> b >> s;
        pair<int, int> g = get_grundy(b, s);
        x.first ^= g.first;
        x.second ^= g.second;
    }

    if(x.first || x.second) cout << "Win\n";
    else cout << "Loss\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> k >> q;
    if(k) calc_grundy();
    while(q--) solve();
}