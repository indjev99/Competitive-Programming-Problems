#include<iostream>
#define MAXN 5000
using namespace std;
long long int a[MAXN][MAXN];
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,m;
    cin >> n >> m ;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            a[i][j]=1e18;
        }
    }
    for (int i=0; i<m; i++) {
        int u,v,w;
        cin >> u >> v >> w ;
        a[u][v]=w;
    }
    for (int middle=0; middle<n; middle++) {
        for (int st=0; st<n; st++) {
            for (int fin=0; fin<n; fin++) {
                if (a[st][fin]>a[st][middle]+a[middle][fin]) a[st][fin]=a[st][middle]+a[middle][fin];
            }
        }
    }
    for (int i=0; i<n; i++) {
        if (a[i][i]<=0) {
            cout << 1 << endl ;
            return 0;
        }
    }
    cout << 0 << endl ;
    return 0;
}
