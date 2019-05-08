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
std::pair<long long, int> rmqStack[MAX_M];
int rmqPtr=0;
void update_rmq(int pos, long long val)
{
    while (rmqPtr && rmqStack[rmqPtr-1].first>=val) --rmqPtr;
    rmqStack[rmqPtr++]={val,pos};
}
void init_rmq()
{
    update_rmq(a,0);
}
long long rmq(int left, int right)
{
    int l=0,r=rmqPtr,m;
    while (l<r)
    {
        m=(l+r)/2;
        if (rmqStack[m].second<left) l=m+1;
        else r=m;
    }
    if (l==rmqPtr) return INF;
    return rmqStack[l].first;
}
void solve()
{
    std::sort(is,is+n);
    std::fill(cost+a+1,cost+maxCoord+1,INF);
    init_rmq();
    ans=INF;
    for (int i=0;i<n;++i)
    {
        long long currCost=is[i].cost+rmq(is[i].from,is[i].to);
        if (currCost<cost[is[i].to])
        {
            cost[is[i].to]=currCost;
            update_rmq(is[i].to,currCost);
        }
        if (is[i].to>=b && currCost<ans) ans=currCost;
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
