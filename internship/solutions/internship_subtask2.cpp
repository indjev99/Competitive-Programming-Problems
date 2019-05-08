#include <iostream>
#include <algorithm>

const long long INF=1e18;
const int MAX_N=5e5;
const int MAX_M=1e7+2;

struct Interval
{
    int from;
    int to;
    long long cost;
};
bool operator<(const Interval& i1, const Interval& i2)
{
    return i1.to<i2.to;
}
Interval is[MAX_N];
long long cost[MAX_M];
int a,b;
int n;
long long ans;
int maxCoord;

void input()
{
    std::cin>>n;
    std::cin>>a>>b;
    ++b;
    maxCoord=b;
    for (int i=0;i<n;++i)
    {
        std::cin>>is[i].from>>is[i].to>>is[i].cost;
        ++is[i].to;
        if (is[i].to>maxCoord) maxCoord=is[i].to;
    }
}
void output()
{
    if (ans<INF) std::cout<<ans<<'\n';
    else std::cout<<"Impossible\n";
}
long long rmq(int left, int right)
{
    long long m=INF;
    for (int i=left;i<right;++i)
    {
        if (cost[i]<m) m=cost[i];
    }
    return m;
}
void solve()
{
    std::sort(is,is+n);
    std::fill(cost+a+1,cost+maxCoord+1,INF);
    for (int i=a+1;i<=maxCoord;++i)
    {
        for (int j=0;j<n;++j)
        {
            if (is[j].from>=i || is[j].to<i) continue;
            long long currCost=is[j].cost+rmq(is[j].from,is[j].to);
            if (currCost<cost[i]) cost[i]=currCost;
        }
    }
    ans=cost[b];
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
