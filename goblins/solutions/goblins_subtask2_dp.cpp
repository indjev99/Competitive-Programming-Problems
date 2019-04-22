#include <iostream>
#include <algorithm>

const int MAX_N=1e6;
const int MAX_Q=1e6;
const int MAX_T=5e6;

struct Query
{
    int num;
    int t;
    bool operator<(const Query& other) const
    {
        return t<other.t;
    }
};

int n,q;
int a[MAX_N];
Query qs[MAX_Q];
int fs;
long long ans[MAX_Q];

void output()
{
    for (int i=0;i<q;++i)
    {
        std::cout<<ans[i]<<'\n';
    }
}
void input()
{
    std::cin>>n>>q;
    for (int i=0;i<n;++i)
    {
        std::cin>>a[i];
    }
    for (int i=0;i<q;++i)
    {
        qs[i].num=i;
        std::cin>>qs[i].t;
    }
}
long long dp[2][MAX_N];
bool currDp=false;
void updateDp()
{
    dp[!currDp][0]=dp[currDp][1]+a[0];
    for (int i=1;i<n-1;++i)
    {
        dp[!currDp][i]=std::max(dp[currDp][i-1],dp[currDp][i+1])+a[i];
    }
    dp[!currDp][n-1]=dp[currDp][n-2]+a[n-1];
    currDp=!currDp;
}
void solve()
{
    int curr=0;
    std::sort(qs,qs+q);
    for (int i=0;i<q;++i)
    {
        while (curr<qs[i].t)
        {
            updateDp();
            ++curr;
        }
        ans[qs[i].num]=dp[currDp][0];
    }
}
int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}
