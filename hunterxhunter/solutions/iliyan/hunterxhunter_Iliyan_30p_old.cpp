#include<iostream>
#include<string.h>
#include<vector>
#define MAXN 10000
using namespace std;
int people[MAXN][2];
int dp[2][2][2*MAXN];
vector <int> cycle;
inline int max (int num1, int num2) {
    return (num1>num2)?num1:num2;
}
int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,k,i,j,curr,prev,currp,ans=-1e9;
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

    curr=(n-1)%2;
    for (i=0; i<=2*n; i++) {
        dp[curr][0][i]=dp[curr][1][i]=-1e9;
        }

    dp[curr][0][n+1]=dp[curr][1][n+1]=0;
    dp[curr][1][n-k+1]=people[cycle[n-1]][1];
    dp[curr][1][n-1]=people[cycle[n-1]][1];

    for (i=n-2; i>=0; i--) {
        curr=i%2;
        prev=curr^1;

        for (j=1; j<2*n; j++) {
            dp[curr][0][j]=dp[prev][1][j-1];
            if (j+2*k-1<2*n) dp[curr][0][j]=max(dp[curr][0][j],dp[prev][1][j+2*k-1]+people[cycle[i]][1]);

            dp[curr][1][j]=max(dp[curr][0][j],dp[prev][1][j]);
            if (j+1<2*n) dp[curr][1][j]=max(dp[curr][1][j],dp[prev][0][j+1]+people[cycle[i]][1]);
            if (j+k<2*n) dp[curr][1][j]=max(dp[curr][1][j],dp[prev][1][j+k]+people[cycle[i]][1]);
            }
        }

    for (j=n; j<2*n; j++) {
        ans=max(ans,dp[0][1][j]);
        }
    cout << ans ;
    cout << endl ;
    return 0;
}
