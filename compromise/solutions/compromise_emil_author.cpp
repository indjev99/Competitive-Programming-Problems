#include <iostream>
#include <stack>

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

int lastTime[MAX_N];
int parent[MAX_N];
int setSize[MAX_N];
int components;
int timestamp=0;
void clear_sets()
{
    ++timestamp;
    components=n;
}
int find_set(int a)
{
    if (lastTime[a]<timestamp)
    {
        lastTime[a]=timestamp;
        parent[a]=-1;
        setSize[a]=1;
        return a;
    }
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

struct Range
{
    int left,right;
    int leftChild,rightChild;
    int size() const
    {
        return right-left+1;
    }
    void finish();
};
Range ranges[MAX_M+1];
void Range::finish()
{
    if (left==m+1) left=0;
    if (leftChild==-1 || (rightChild>=0 && ranges[leftChild].size()<ranges[rightChild].size())) std::swap(leftChild,rightChild);
}

std::stack<int> s;
void add_to_stack(int pos)
{
    ranges[pos].leftChild=-1;
    ranges[pos].rightChild=-1;
    int basePos;
    while (edges[pos].time>edges[s.top()].time)
    {
        basePos=s.top();
        s.pop();
        ranges[basePos].left=s.top()+1;
        ranges[basePos].right=pos-1;
        ranges[pos].leftChild=basePos;
        ranges[s.top()].rightChild=basePos;
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

int currLeft,currRight;
void DFS(int pos)
{
    if (ranges[pos].rightChild!=-1) DFS(ranges[pos].rightChild);
    if (ranges[pos].leftChild!=-1) DFS(ranges[pos].leftChild);
    else
    {
        clear_sets();
        currLeft=pos;
        currRight=pos-1;
    }
    while (currLeft>ranges[pos].left)
    {
        --currLeft;
        unite_sets(edges[currLeft].from,edges[currLeft].to);
    }
    while (currRight<ranges[pos].right)
    {
        ++currRight;
        unite_sets(edges[currRight].from,edges[currRight].to);
    }
    long long currAns;
    unite_sets(edges[pos].from,edges[pos].to);
    currAns=edges[pos].time*components;
    if (currAns<ans || ans<0) ans=currAns;
}
void solve()
{
    find_ranges();
    DFS(ranges[m].leftChild);
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
