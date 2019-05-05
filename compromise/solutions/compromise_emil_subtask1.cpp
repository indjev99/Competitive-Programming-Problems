#include <iostream>

const int MAX_N=5e5;
const int MAX_M=5e5;
const long long MAX_TIME=1e12;

struct Edge
{
    int from,to;
    long long time;
};

int n,m;
Edge edges[MAX_M];
long long ans=-1;

void input()
{
    std::cin>>n>>m;
    for (int i=0;i<m;++i)
    {
        std::cin>>edges[i].from>>edges[i].to>>edges[i].time;
        --edges[i].from;
        --edges[i].to;
    }
}
void output()
{
    std::cout<<ans<<std::endl;
}

int parent[MAX_N];
int setSize[MAX_N];
int components;
void clear_sets()
{
    std::fill(parent,parent+n,-1);
    std::fill(setSize,setSize+n,1);
    components=n;
}
int find_set(int a)
{
    if (parent[a]==-1) return a;
    return parent[a]=find_set(parent[a]);
}
void unite_sets(int a, int b)
{
    a=find_set(a);
    b=find_set(b);
    if (a!=b)
    {
        --components;
        if (setSize[a]>=setSize[b])
        {
            parent[b]=a;
            setSize[a]=setSize[a]+setSize[b];
        }
        else
        {
            parent[a]=b;
            setSize[b]=setSize[a]+setSize[b];
        }
    }
}

void solve()
{
    long long maxTime;
    long long currAns;
    for (int i=0;i<m;++i)
    {
        clear_sets();
        maxTime=0;
        for (int j=i;j<m;++j)
        {
            if (edges[j].time>maxTime) maxTime=edges[j].time;
            unite_sets(edges[j].from,edges[j].to);
            currAns=maxTime*components;
            if (currAns<ans || ans<0) ans=currAns;
        }
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
