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
long long rmqArr[1<<25];
void util_init_rmq(int node, int left, int right)
{
    if (left==right)
    {
        rmqArr[node]=cost[left];
        return;
    }
    int mid=(left+right)/2;
    util_init_rmq(node*2+1,left,mid);
    util_init_rmq(node*2+2,mid+1,right);
    rmqArr[node]=std::min(rmqArr[node*2+1],rmqArr[node*2+2]);
}
void util_update_rmq(int node, int left, int right, int pos, long long val)
{
    if (pos>right || pos<left) return;
    if (left==right)
    {
        rmqArr[node]=val;
        return;
    }
    int mid=(left+right)/2;
    util_update_rmq(node*2+1,left,mid,pos,val);
    util_update_rmq(node*2+2,mid+1,right,pos,val);
    rmqArr[node]=std::min(rmqArr[node*2+1],rmqArr[node*2+2]);
}
long long util_rmq(int node, int left, int right, int rangeLeft, int rangeRight)
{
    if (left>rangeRight || right<rangeLeft) return INF;
    if (left>=rangeLeft && right<=rangeRight) return rmqArr[node];
    int mid=(left+right)/2;
    return std::min(util_rmq(node*2+1,left,mid,rangeLeft,rangeRight),
                    util_rmq(node*2+2,mid+1,right,rangeLeft,rangeRight));
}
void init_rmq()
{
    util_init_rmq(0,0,maxCoord);
}
void update_rmq(int pos, long long val)
{
    util_update_rmq(0,0,maxCoord,pos,val);
}
long long rmq(int left, int right)
{
    return util_rmq(0,0,maxCoord,left,right-1);
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
