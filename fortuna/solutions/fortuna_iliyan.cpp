#include "fortuna.h"
#include<iostream>
#include<numeric>
#define MAXN 30
#define MAXM 30
#define MAXB 10000
using namespace std;
int a[MAXB][MAXN];
int terminal[MAXB];
int used[MAXM];
void setup (int n, int m) {
    int cnt=1+m;
    for (int i=1; i<=m; i++) {
        terminal[i]=i;
    }
    used[1]=1;
    int level=1;
    for (int it=0; ; it++) {
        int nxt=level*n,rem=nxt%m;
        //cout << level << " " << rem << endl ;
        int curr=0,c=0;
        int st=cnt-level,fin=cnt-1;
        if (it==0) {
            for (int i=0; i<n; i++) {
                if (c<rem) a[0][i]=cnt++;
                else {
                    a[0][i]=1+(curr++);
                    curr%=m;
                }
                c++;
            }
        }
        else {
            for (int i=st; i<=fin; i++) {
                for (int j=0; j<n; j++) {
                    if (c<rem) a[i][j]=cnt++;
                    else {
                        a[i][j]=1+(curr++);
                        curr%=m;
                    }
                    c++;
                }
            }
        }
        if (rem==0) break;
        if (used[rem]>0) {
            c=0; used[rem]--;
            for (int i=st; i<=fin; i++) {
                for (int j=0; j<n; j++) {
                    if (c<rem) a[i][j]=used[rem]++;
                    c++;
                }
            }
            break;
        }
        used[rem]=1+(cnt-rem);
        level=rem;
    }
    /*for (int i=0; i<cnt; i++) {
        if (terminal[i]!=0) cout << terminal[i]-1 << endl ;
        else {
            for (int j=0; j<n; j++) {
                cout << a[i][j] << " ";
            }
            cout << endl ;
        }
    }*/
}
pair <bool, int> proc (int x, int r) {
    int nxt=a[x][r];
    if (terminal[nxt]!=0) return {true, terminal[nxt]-1};
    return {false, nxt};
}
