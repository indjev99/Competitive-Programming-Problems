#include<iostream>
#include<vector>
#include<queue>
#define MAXN 5000
using namespace std;
vector <pair <int, long long int>> a[MAXN+1];
long long int dist[MAXN+1];
int len[MAXN+1];
bool used[MAXN+1];
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,m;
    cin >> n >> m ;
    for (int i=0; i<m; i++) {
        int u,v;
        long long int w;
        cin >> u >> v >> w ;
        u++; v++;
        w*=n; w--;
        a[u].push_back({v, w});
    }
    for (int i=0; i<=n; i++) {
        a[0].push_back({i, 0});
        dist[i]=1e18;
    }
    queue <int> spfa;
    dist[0]=0; len[0]=0;
    spfa.push(0);
    used[0]=true;
    for (;;) {
        if (spfa.empty()==true) break;
        auto vr=spfa.front();
        spfa.pop();
        used[vr]=false;
        for (auto [to, w] : a[vr]) {
            if (dist[to]>dist[vr]+w) {
                dist[to]=dist[vr]+w; len[to]=len[vr]+1;
                if (len[to]>=n+1) {
                    cout << 1 << endl ;
                    return 0;
                }
                if (used[to]==false) {
                    spfa.push(to);
                    used[to]=true;
                }
            }
        }
    }
    cout << 0 << endl ;
    return 0;
}
