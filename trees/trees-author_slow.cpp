#include<iostream>
#include<fstream>
#include<stack>
#include<queue>
using namespace std;

#define official
#ifdef official
ifstream inF("trees.in");
ofstream outF("trees.out");
#define cin inF
#define cout outF
#endif

struct Change
{
    int f, t;
    int node1;
    int treeSize1;
    int node2;
    int par2;
    int product;
};
const int MAX_N=1e5;
const int MAX_Q=1e5;
const long long MOD=1e9+7;
int n,q;
int par[MAX_N];
int treeSize[MAX_N];
stack<Change> changes;
long long product;
int components;
void input()
{
    cin>>n>>q;
}
void init()
{
    for (int i=0;i<n;++i)
    {
        par[i]=i;
        treeSize[i]=1;
    }
    product=1;
    components=n;
}
long long qpow(long long x, long long k)
{
    if (k==0) return 1;
    if (k%2) return x*qpow(x,k-1)%MOD;
    long long ans=qpow(x,k/2);
    return ans*ans%MOD;
}
long long modInv(long long x)
{
    return qpow(x,MOD-2);
}
long long compute()
{
    if (components==1) return 1;
    return qpow(n,components-2)*product%MOD;
}
int findRoot(int p)
{
    while (par[p]!=p) p=par[p];
    return p;
}
void addEdge(int f, int t)
{
    if (f>t) swap(f,t);
    Change curr;
    curr.f=f;
    curr.t=t;
    int p1=findRoot(f);
    int p2=findRoot(t);
    if (p1==p2) cerr<<"ERROR 2"<<endl;
    if (treeSize[p1]<treeSize[p2]) swap(p1,p2);
    curr.node1=p1;
    curr.treeSize1=treeSize[p1];
    curr.node2=p2;
    curr.par2=par[p2];
    curr.product=product;
    changes.push(curr);
    --components;
    par[p2]=p1;
    product*=modInv(treeSize[p1]*treeSize[p2]%MOD);
    product%=MOD;
    treeSize[p1]+=treeSize[p2];
    product*=treeSize[p1];
    product%=MOD;
}
queue<pair<int,int>> toProcess;
void removeEdge(int f, int t)
{
    if (f>t) swap(f,t);
    Change curr;
    while (1)
    {
        if (changes.empty()) cerr<<"ERROR 3"<<endl;
        Change curr=changes.top();
        changes.pop();
        treeSize[curr.node1]=curr.treeSize1;
        par[curr.node2]=curr.par2;
        product=curr.product;
        ++components;
        if (curr.f!=f || curr.t!=t) toProcess.push({curr.f,curr.t});
        else break;
    }
    while (!toProcess.empty())
    {
        f=toProcess.front().first;
        t=toProcess.front().second;
        toProcess.pop();
        addEdge(f,t);
    }
}
int main()
{
    input();
    init();
    int type,f,t;
    for (int i=0;i<q;++i)
    {
        cin>>type>>f>>t;
        --f;
        --t;
        if (type==1) addEdge(f,t);
        else if (type==2) removeEdge(f,t);
        else cerr<<"ERROR 1"<<endl;
        cout<<compute()<<'\n';
    }
    return 0;
}
