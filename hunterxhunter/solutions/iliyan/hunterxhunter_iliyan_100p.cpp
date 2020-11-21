#include<iostream>
#include<string.h>
#include<vector>
#define MAXN 10000
using namespace std;
int people[MAXN][2];
int dp[3][2][2*MAXN],seen[MAXN];
vector <vector <int> >cycles;
inline int max (int num1, int num2) {
    return (num1>num2)?num1:num2;
}
int main () {
    //ios_base::sync_with_stdio(false);
    //cin.tie(NULL);
    int n,k,i,j,curr,prev[2],currp,ans=-1e9,first,cnt=0,m;
    cin >> n >> k ;
    for (i=0; i<n; i++) {
        cin >> people[i][0] >> people[i][1] ;
        }
    for (i=0; i<n; i++) {
        if (seen[i]!=0) continue;
        currp=i;
        cycles.push_back({});
        for (;;) {
            seen[currp]=1;
            cycles[cnt].push_back(currp);
            currp=people[currp][0];
            if (currp==i) break;
            }
        cnt++;
        }

    for (first=0; first<2; first++) {
        for (i=0; i<2*n; i++) {
            dp[0][first][i]=-1e9;
            }
        }
    dp[0][0][n]=dp[0][1][n]=0;

    for (auto cycle : cycles) {
        m=cycle.size();

        curr=m%3;
        for (first=0; first<2; first++) {
            for (i=0; i<2*n; i++) {
                dp[curr][first][i]=max(dp[0][0][i],dp[0][1][i]);
                }
            }

        curr=(m-1)%3;
        for (i=0; i<2*n; i++) {
            dp[curr][0][i]=dp[curr][1][i]=dp[(m%3)][0][i];
            if (i>0) dp[curr][0][i]=max(dp[curr][0][i],dp[(m%3)][0][i-1]);
            if (i+k<2*n) dp[curr][0][i]=max(dp[curr][0][i],dp[(m%3)][0][i+k]+people[cycle[m-1]][1]);

            if (i+k<2*n) dp[curr][1][i]=max(dp[curr][1][i],dp[(m%3)][1][i+k]+people[cycle[m-1]][1]);
            if (i+1<2*n) dp[curr][1][i]=max(dp[curr][1][i],dp[(m%3)][1][i+1]+people[cycle[m-1]][1]);

            //if (dp[curr][0][i]>=0) cout << i << " " << dp[curr][0][i] << endl ;
            //if (dp[curr][1][i]>=0) cout << i << " " << dp[curr][1][i] << endl ;
            }

        for (i=m-2; i>=0; i--) {
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
