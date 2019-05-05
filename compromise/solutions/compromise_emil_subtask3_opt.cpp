#include <iostream>
#include <algorithm>
#include <stack>

const int MAX_N=5e5;
const int MAX_M=5e5;
const int BUCKET_SIZE=300;
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

struct Change
{
    int node,nodeParent;
    int root,rootSetSize;
    void revert()
    {
        parent[node]=nodeParent;
        setSize[root]=rootSetSize;
        ++components;
    }
};
std::stack<Change> changes;

void clear_sets()
{
    std::fill(parent,parent+n,-1);
    std::fill(setSize,setSize+n,1);
    components=n;
}
int find_set(int a)
{
    if (parent[a]==-1) return a;
    return find_set(parent[a]);
}
int find_set_reroot(int a)
{
    if (parent[a]==-1) return a;
    return parent[a]=find_set(parent[a]);
}
void unite_sets(int a, int b, bool toRevert)
{
    if (toRevert)
    {
        a=find_set(a);
        b=find_set(b);
    }
    else
    {
        a=find_set_reroot(a);
        b=find_set_reroot(b);
    }
    if (a!=b)
    {
        --components;
        if (setSize[a]>=setSize[b])
        {
            if (toRevert) changes.push({b,parent[b],a,setSize[a]});
            parent[b]=a;
            setSize[a]=setSize[a]+setSize[b];
        }
        else
        {
            if (toRevert) changes.push({a,parent[a],b,setSize[b]});
            parent[a]=b;
            setSize[b]=setSize[a]+setSize[b];
        }
    }
}
void revert_all()
{
    while (!changes.empty())
    {
        changes.top().revert();
        changes.pop();
    }
}

struct Range
{
    int left,right;
    long long maxTime;
    void finish()
    {
        if (left==m+1) left=0;
    }
    int bucket() const
    {
        return left/BUCKET_SIZE;
    }
    bool operator<(const Range& other) const
    {
        if (bucket()!=other.bucket()) return bucket()<other.bucket();
        return right<other.right;
    }
};
Range ranges[MAX_M];

std::stack<int> s;
void add_to_stack(int pos)
{
    int basePos;
    while (edges[pos].time>edges[s.top()].time)
    {
        basePos=s.top();
        s.pop();
        ranges[basePos].left=s.top()+1;
        ranges[basePos].right=pos-1;
        ranges[basePos].maxTime=edges[basePos].time;
    }
    s.push(pos);
}
void find_ranges()
{
    edges[m].time=MAX_TIME+1;
    s.push(m);
    for (int i=0;i<m;++i)
    {
        add_to_stack(i);
    }
    add_to_stack(m);
    for (int i=0;i<m;++i)
    {
        ranges[i].finish();
    }
}

void solve()
{
    find_ranges();
    std::sort(ranges,ranges+m);
    int lastBucket=-1;
    int currLeft,currRight;
    long long currAns;
    for (int i=0;i<m;++i)
    {
        if (ranges[i].bucket()!=lastBucket)
        {
            lastBucket=ranges[i].bucket();
            clear_sets();
            currLeft=std::min((lastBucket+1)*BUCKET_SIZE,n);
            currRight=currLeft-1;
        }
        while (currRight<ranges[i].right)
        {
            ++currRight;
            unite_sets(edges[currRight].from,edges[currRight].to,false);
        }
        for (int j=ranges[i].left;j<std::min(currLeft,ranges[i].right+1);++j)
        {
            unite_sets(edges[j].from,edges[j].to,true);
        }
        currAns=ranges[i].maxTime*components;
        if (currAns<ans || ans<0) ans=currAns;
        revert_all();
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
