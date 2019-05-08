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
int eventNumber[MAX_M];
int eventList[MAX_N*2+2];
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
    int leftEvent=eventNumber[left];
    int rightEvent=eventNumber[right];
    for (int i=leftEvent;i<rightEvent;++i)
    {
        if (cost[eventList[i]]<m) m=cost[eventList[i]];
    }
    return m;
}
void create_events()
{
    for (int i=0;i<n;++i)
    {
        eventList[2*i]=is[i].from;
        eventList[2*i+1]=is[i].to;
    }
    eventList[2*n]=a;
    eventList[2*n+1]=b;
    std::sort(eventList,eventList+2*n+2);
    for (int i=0;i<2*n+2;++i)
    {
        if (!i || eventList[i]!=eventList[i-1])
        {
            eventNumber[eventList[i]]=i;
        }
    }
}
void solve()
{
    std::sort(is,is+n);
    std::fill(cost+a+1,cost+maxCoord+1,INF);
    create_events();
    ans=INF;
    for (int i=0;i<n;++i)
    {
        long long currCost=is[i].cost+rmq(is[i].from,is[i].to);
        if (currCost<cost[is[i].to]) cost[is[i].to]=currCost;
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
