#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

#define official
#ifdef official
ifstream inF("bases.in");
ofstream outF("bases.out");
#define cin inF
#define cout outF
#endif

const long long MOD=1e9+7;
const int MAX_N=20;
const int MAX_NUM=1e6;
int n,k,q;
long long dp[MAX_NUM+1];
vector<int> digs;

void input()
{
    cin>>n>>k;
    int t;
    for (int i=0;i<k;++i)
    {
        cin>>t;
        digs.push_back(t);
    }
    cin>>q;
}
void solve()
{
    dp[0]=1;
    int d;
    for (int i=1;i<=MAX_NUM;++i)
    {
        dp[i]=0;
        for (int j=0;j<digs.size();++j)
        {
            d=digs[j];
            if (d>i || d%n!=i%n) continue;
            dp[i]+=dp[(i-d)/n];
            dp[i]%=MOD;
        }
    }
}
int main()
{
    input();
    solve();
    int x;
    for (int i=0;i<q;++i)
    {
        cin>>x;
        if (i) cout<<' ';
        cout<<dp[x];
    }
    cout<<'\n';
    return 0;
}
