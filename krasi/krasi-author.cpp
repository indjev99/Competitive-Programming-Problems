#include<iostream>
#include<fstream>
using namespace std;

//#define official
#ifdef official
ifstream inF("krasi.in");
ofstream outF("krasi.out");
#define cin inF
#define cout outF
#endif

const int MAX_N=100000;
const int MAX_K=100;
int n,k;
long long arr[MAX_N];
long long ans;
long long dp[MAX_N+1][MAX_K+1];

void input()
{
    cin>>n>>k;
    for (int i=0;i<n;++i)
    {
        cin>>arr[i];
    }
}
void output()
{
    cout<<ans<<endl;
}
void solve()
{
    ans=arr[0];
    for (int j=0;j<=k;++j)
    {
        dp[0][j]=0;
    }
    for (int i=0;i<n;++i)
    {
        if (arr[i]==0)
        {
            for (int j=1;j<=k;++j)
            {
                dp[i+1][j]=dp[i][j-1];
            }
            dp[i+1][0]=0;
            if (dp[i+1][0]>ans) ans=0;
        }
        else
        {
            for (int j=0;j<=k;++j)
            {
                dp[i+1][j]=dp[i][j]+arr[i];
                if (dp[i+1][j]>ans) ans=dp[i+1][j];
                if (dp[i+1][j]<0) dp[i+1][j]=0;
            }
        }
    }
}
int main()
{
    input();
    solve();
    output();
    return 0;
}
