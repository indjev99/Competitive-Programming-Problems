#include<iostream>
#include<string.h>
#include<vector>
#define MAXN 10000
using namespace std;
int people[MAXN][2];
int dp[3][2][2*MAXN];
vector <int> cycle;
inline int max (int num1, int num2) {
    return (num1>num2)?num1:num2;
}
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,k,i,j,curr,prev[2],currp,ans=-1e9,first;
    cin >> n >> k ;
    for (i=0; i<n; i++) {
        cin >> people[i][0] >> people[i][1] ;
        }
    currp=0;
    for (;;) {
        cycle.push_back(currp);
        currp=people[currp][0];
        if (currp==0) break;
        }

    for (curr=0; curr<3; curr++) {
        for (first=0; first<2; first++) {
            for (i=0; i<2*n; i++) {
                dp[curr][first][i]=-1e9;
                }
            }
        }

    curr=n%3;
    dp[curr][0][n]=dp[curr][1][n]=0;

    curr=(n-1)%3;
    dp[curr][0][n+1]=0;
    dp[curr][0][n-k]=people[cycle[n-1]][1];

    dp[curr][1][n-k]=people[cycle[n-1]][1];
    dp[curr][1][n-1]=people[cycle[n-1]][1];

    for (i=n-2; i>=0; i--) {
        curr=i%3;
        prev[0]=(curr+1)%3;
        prev[1]=(curr+2)%3;

        for (first=0; first<2; first++) {
            for (j=1; j<2*n; j++) {
                dp[curr][first][j]=max(dp[prev[0]][first][j-1],dp[prev[0]][first][j]);
                if (j+2*k<2*n) dp[curr][first][j]=max(dp[curr][first][j],dp[prev[0]][first][j+2*k]+people[cycle[i]][1]);

                if ((i>0)||((i==0)&&(first==0))) {
                    if (j<2*n) dp[curr][first][j]=max(dp[curr][first][j],dp[prev[1]][first][j]+people[cycle[i]][1]);
                    if (j+k<2*n) dp[curr][first][j]=max(dp[curr][first][j],dp[prev[0]][first][j+k]+people[cycle[i]][1]);
                    }
                }
            }
        }

    for (first=0; first<2; first++) {
        for (j=n; j<2*n; j++) {
            ans=max(ans,dp[0][first][j]);
            }
        }
    cout << ans ;
    cout << endl ;
    return 0;
}
